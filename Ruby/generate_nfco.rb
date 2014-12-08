#! /usr/bin/env ruby

location = File.dirname __FILE__
$: << "#{location}"

require 'repository'
require 'organization_service'
require 'conference_service'
require 'team_service'
require 'player_service'
require 'positions'
require 'player_generator'
require 'organization'
require 'organization_conference'
require 'conference'
require 'conference_team'
require 'team'
require 'team_player'


if ARGV.length != 1
  abort "Usage: #{__FILE__} <filename>\n"
end


@organization = { organization_id: 1, name: 'National College Football Organization', abbreviation: 'NCFO', season: 1 }

@conferences = [ { conference_id: 1, name: 'Atlantic'    },
                 { conference_id: 2, name: 'Great Lake'  },
                 { conference_id: 3, name: 'Midwest'     },
                 { conference_id: 4, name: 'New England' },
                 { conference_id: 5, name: 'North'       },
                 { conference_id: 6, name: 'Pacific'     },
                 { conference_id: 7, name: 'Southeast'   },
                 { conference_id: 8, name: 'Southwest'   } ]

@teams = [
          # Atlantic
          { team_id:  1, name: 'Blue Hens',       location: 'Delaware',       abbreviation: 'UDE' },
          { team_id:  2, name: 'Terrapins',       location: 'Maryland',       abbreviation: 'UMD' },
          { team_id:  3, name: 'Scarlet Knights', location: 'New Jersey',     abbreviation: 'UNJ' },
          { team_id:  4, name: 'Nittany Lions',   location: 'Pennsylvania',   abbreviation: 'UPA' },
          { team_id:  5, name: 'Cavaliers',       location: 'Virginia',       abbreviation: 'UVA' },
          { team_id:  6, name: 'Mountaineers',    location: 'West Virginia',  abbreviation: 'UWV' },
          # Great Lake
          { team_id:  7, name: 'Fighting Illini', location: 'Illinois',       abbreviation: 'UIL' },
          { team_id:  8, name: 'Wolverines',      location: 'Michigan',       abbreviation: 'UMI' },
          { team_id:  9, name: 'Golden Gophers',  location: 'Minnesota',      abbreviation: 'UMN' },
          { team_id: 10, name: 'Bobcats',         location: 'New York',       abbreviation: 'UNY' },
          { team_id: 11, name: 'Buckeyes',        location: 'Ohio',           abbreviation: 'UOH' },
          { team_id: 12, name: 'Badgers',         location: 'Wisconsin',      abbreviation: 'UWI' },
          # Midwest
          { team_id: 13, name: 'Razorbacks',      location: 'Arkansas',       abbreviation: 'UAR' },
          { team_id: 14, name: 'Hoosiers',        location: 'Indiana',        abbreviation: 'UIN' },
          { team_id: 15, name: 'Jayhawks',        location: 'Kansas',         abbreviation: 'UKS' },
          { team_id: 16, name: 'Wildcats',        location: 'Kentucky',       abbreviation: 'UKY' },
          { team_id: 17, name: 'Tigers',          location: 'Missouri',       abbreviation: 'UMO' },
          { team_id: 18, name: 'Volunteers',      location: 'Tennessee',      abbreviation: 'UTN' },
          # New England
          { team_id: 19, name: 'Huskies',         location: 'Connecticut',    abbreviation: 'UCT' },
          { team_id: 20, name: 'Black Bears',     location: 'Maine',          abbreviation: 'UME' },
          { team_id: 21, name: 'Minutemen',       location: 'Massachusetts',  abbreviation: 'UMA' },
          { team_id: 22, name: 'Big Green',       location: 'New Hampshire',  abbreviation: 'UNH' },
          { team_id: 23, name: 'Rams',            location: 'Rhode Island',   abbreviation: 'URI' },
          { team_id: 24, name: 'Catamounts',      location: 'Vermont',        abbreviation: 'UVT' },
          # North
          { team_id: 25, name: 'Hawkeyes',        location: 'Iowa',           abbreviation: 'UIA' },
          { team_id: 26, name: 'Grizzlies',       location: 'Montana',        abbreviation: 'UMT' },
          { team_id: 27, name: 'Corn Huskers',    location: 'Nebraska',       abbreviation: 'UNE' },
          { team_id: 28, name: 'Fighting Sioux',  location: 'North Dakota',   abbreviation: 'UND' },
          { team_id: 29, name: 'Coyotes',         location: 'South Dakota',   abbreviation: 'USD' },
          { team_id: 30, name: 'Cowboys',         location: 'Wyoming',        abbreviation: 'UWY' },
          # Pacific
          { team_id: 31, name: 'Golden Bears',    location: 'California',     abbreviation: 'UCA' },
          { team_id: 32, name: 'Vandals',         location: 'Idaho',          abbreviation: 'UID' },
          { team_id: 33, name: 'Wolf Pack',       location: 'Nevada',         abbreviation: 'UNV' },
          { team_id: 34, name: 'Ducks',           location: 'Oregon',         abbreviation: 'UOR' },
          { team_id: 35, name: 'Utes',            location: 'Utah',           abbreviation: 'UUT' },
          { team_id: 36, name: 'Cougars',         location: 'Washington',     abbreviation: 'UWA' },
          # Southeast
          { team_id: 37, name: 'Crimson Tide',    location: 'Alabama',        abbreviation: 'UAL' },
          { team_id: 38, name: 'Gators',          location: 'Florida',        abbreviation: 'UFL' },
          { team_id: 39, name: 'Bulldogs',        location: 'Georgia',        abbreviation: 'UGA' },
          { team_id: 40, name: 'Rebels',          location: 'Mississippi',    abbreviation: 'UMS' },
          { team_id: 41, name: 'Tarheels',        location: 'North Carolina', abbreviation: 'UNC' },
          { team_id: 42, name: 'Gamecocks',       location: 'South Carolina', abbreviation: 'USC' },
          # Southwest
          { team_id: 43, name: 'Sun Devils',      location: 'Arizona',        abbreviation: 'UAZ' },
          { team_id: 44, name: 'Buffalos',        location: 'Colorado',       abbreviation: 'UCO' },
          { team_id: 45, name: 'Ragin Cajuns',    location: 'Louisiana',      abbreviation: 'ULA' },
          { team_id: 46, name: 'Lobos',           location: 'New Mexico',     abbreviation: 'UNM' },
          { team_id: 47, name: 'Sooners',         location: 'Oklahoma',       abbreviation: 'UOK' },
          { team_id: 48, name: 'Longhorns',       location: 'Texas',          abbreviation: 'UTX' },
         ]

@all_american_teams = [
                       { team_id: 49, name: 'American', location: 'American', abbreviation: 'AME' },
                       { team_id: 50, name: 'National', location: 'National', abbreviation: 'NAT' }
                      ]

@positions = [
              Positions::Quarterback,
              Positions::Quarterback,
              Positions::Runningback,
              Positions::Runningback,
              Positions::Runningback,
              Positions::Runningback,
              Positions::WideReceiver,
              Positions::WideReceiver,
              Positions::WideReceiver,
              Positions::WideReceiver,
              Positions::WideReceiver,
              Positions::WideReceiver,
              Positions::TightEnd,
              Positions::TightEnd,
              Positions::OffensiveLine,
              Positions::OffensiveLine,
              Positions::OffensiveLine,
              Positions::OffensiveLine,
              Positions::OffensiveLine,
              Positions::DefensiveLine,
              Positions::DefensiveLine,
              Positions::DefensiveLine,
              Positions::Linebacker,
              Positions::Linebacker,
              Positions::Linebacker,
              Positions::Linebacker,
              Positions::Cornerback,
              Positions::Cornerback,
              Positions::Safety,
              Positions::Safety,
              Positions::Kicker,
              Positions::Punter
             ]

@next_player_id = 0
@pg = PlayerGenerator.new
@prng = Random.new

def get_next_player_id
  @next_player_id += 1
end

def generate_players team_id, positions
  result = []

  positions.each do |pos|
    number = @pg.choose_number pos, result

    freshman_season = @organization[:season] - @prng.rand(4)

    player = @pg.generate_player get_next_player_id, pos, number, freshman_season

    team_player = TeamPlayer.new team_id, @organization[:season], player.player_id

    team_player.player = player

    result.push team_player
  end

  result
end

def generate_teams conference_id, teams
  result = []

  first   = (conference_id - 1) * 6
  last    = first + 6
  current = first

  while current < last do
    team = Team.new

    team.from_hash teams[current]

    conf_team = ConferenceTeam.new conference_id, team.team_id

    conf_team.team = team

    team.players = generate_players team.team_id, @positions

    result.push conf_team

    current += 1
  end

  result
end

def generate_conferences organization_id, conferences
  result = []

  conferences.each_with_index do |conference, index|
    conf = Conference.new

    conf.from_hash conference

    org_conf = OrganizationConference.new organization_id, conf.conference_id

    org_conf.conference = conf

    conf.teams = generate_teams conf.conference_id, @teams

    result.push org_conf
  end

  result
end

ncfo = Organization.new

ncfo.from_hash @organization

ncfo.conferences = generate_conferences ncfo.organization_id, @conferences


filename = ARGV[0]

repo = Repository.new Utils::get_db filename
ps = PlayerService.new repo
ts = TeamService.new repo, ps
cs = ConferenceService.new repo, ts
os = OrganizationService.new repo, cs

aa_team = Team.new


repo.start_transaction

begin
  os.save ncfo

  @all_american_teams.each do |team|
    aa_team.from_hash team
    ts.save aa_team
  end

  repo.end_transaction
  puts "Done."
rescue Exception => e
  repo.cancel_transaction
  raise e
end
