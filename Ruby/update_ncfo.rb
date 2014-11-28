#!/usr/bin/env ruby
#
location = File.dirname __FILE__

$: << "#{location}"
require 'repository'
require 'organization'
require 'team_service'
require 'team_player'
require 'player_service'
require 'player_generator'

def get_team_players_by_season( team_id, season )
  team_player = TeamPlayer.new

  results = @repo.custom_read team_player.build_select_by_team_and_season, { team_id: team_id, season: season }

  if results.nil? or results.length == 0
    raise "No players found for team #{team_id}"
  end

  players = []

  results.each do |result|
    team_player.from_hash result

    unless team_player.player_id.nil?
      player = @repo.read Player.new team_player.player_id

      if player.nil?
        raise "Player not found for id #{team_player.player_id}"
      end

      players.push player
    end

    team_player = TeamPlayer.new
  end

  return players
end

def get_player_class( player, season )
  case season - player.freshman_season
  when 0; return 'Freshman'
  when 1; return 'Sophomore'
  when 2; return 'Junior'
  when 3; return 'Senior'
  else
    raise "Invalid player class #{season - player.freshman_season}"
  end
end

def choose_incoming_class( season )
  return season     unless (@prng.rand(100) + 1) > 95 # Freshman
  return season - 1 unless (@prng.rand(100) + 1) > 95 # Sophomore
  return season - 2 unless (@prng.rand(100) + 1) > 95 # Junior
  return season - 3                                   # Senior
end

def get_next_player_id
  @next_player_id += 1
end


if ARGV.length != 1
  abort "Usage: #{__FILE__} <filename>\n"
end

filename = ARGV[0]

@next_player_id = 0
@repo = Repository.new Utils::get_db filename
@ps = PlayerService.new @repo
@ts = TeamService.new @repo, @ps
@pg = PlayerGenerator.new
@prng = Random.new

@org = Organization.new 1

@org = @repo.read @org

if @org.nil?
  puts "Organization not found with id 1"
  exit
end

@new_season = @org.season + 1

@teams = @ts.get_teams

if @teams.nil? or @teams.length == 0
  puts "No teams found"
  exit
end

result = @repo.custom_read 'SELECT MAX(Player_Id) player_id FROM Players_T'

@next_player_id = result[0][:player_id]

@repo.start_transaction

begin
  @teams.each do |team|
    team.players = get_team_players_by_season team.team_id, @org.season

    players   = []
    graduates = []

    team.players.each do |player|
      player_class = get_player_class player, @org.season

      if player_class == 'Senior'
        graduates.push player
      else
        team_player = TeamPlayer.new team.team_id, @new_season, player.player_id

        @repo.create team_player

        team_player.player = player

        players.push team_player
      end
    end

    graduates.each do |player|
      number = @pg.choose_number player.position, players

      freshman_season = choose_incoming_class @new_season

      new_player = @pg.generate_player get_next_player_id, player.position, number, freshman_season

      @ps.save new_player

      team_player = TeamPlayer.new team.team_id, @new_season, new_player.player_id

      @repo.create team_player

      team_player.player = new_player

      players.push team_player
    end
  end

  @org.season = @new_season

  @repo.update @org
rescue Exception => e
  @repo.cancel_transaction
  raise e
  exit
end

@repo.end_transaction
