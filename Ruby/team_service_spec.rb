location = File.dirname __FILE__
$: << "#{location}"

require 'sqlite3'
require 'repository'
require 'team_service'
require 'player_service'
require 'player'
require 'bowls'
require 'team'
require 'team_accolade'
require 'team_accolades'
require 'team_player'
require 'team_stats'
require 'team_offense_stats'
require 'team_defense_stats'
require 'team_kicking_stats'


db = SQLite3::Database.new 'ncfo_test.db'

db.results_as_hash  = true
db.type_translation = true


describe 'TeamService' do
  describe '#get' do
    before :each do
      @repository    = Repository.new db
      player_service = PlayerService.new @repository
      @team_service  = TeamService.new @repository, player_service

      @repository.start_transaction
    end

    after :each do
      @repository.cancel_transaction
    end

    it 'should return a team with values from the database record' do
      db.execute 'insert into teams_t values (1, "tname", "teamloc", "TN")'

      team = @team_service.get 1

      expect( team ).to_not be_nil
      expect( team ).to     be_a   Team

      expect( team.team_id      ).to eq 1
      expect( team.name         ).to eq 'tname'
      expect( team.location     ).to eq 'teamloc'
      expect( team.abbreviation ).to eq 'TN'
    end

    it 'should return nil if the team is not found' do
      team = @team_service.get 1

      expect( team ).to be_nil
    end

    it 'should return a team with accolades when accolades are available for that team' do
      db.execute 'insert into teams_t values (1, "tname", "teamloc", "TN")'
      db.execute "insert into team_accolades_t values (1, 2, #{TeamAccolades::ConferenceChampions})"
      db.execute "insert into team_accolades_t values (1, 2, #{TeamAccolades::SugarBowlChampions })"

      team = @team_service.get 1

      expect( team ).to_not be_nil
      expect( team ).to     be_a   Team

      expect( team.accolades ).to_not be_nil
      expect( team.accolades ).to     be_a   Array

      expect( team.accolades.length ).to eq 2

      expect( team.accolades[0].team_id   ).to eq 1
      expect( team.accolades[0].season    ).to eq 2
      expect( team.accolades[0].accolade  ).to eq TeamAccolades::ConferenceChampions

      expect( team.accolades[1].team_id   ).to eq 1
      expect( team.accolades[1].season    ).to eq 2
      expect( team.accolades[1].accolade  ).to eq TeamAccolades::SugarBowlChampions
    end

    it 'should return a team without accolades when accolades are not available for that team' do
      db.execute 'insert into teams_t values (1, "tname", "teamloc", "TN")'

      team = @team_service.get 1

      expect( team ).to_not be_nil
      expect( team ).to     be_a   Team

      expect( team.accolades ).to be_nil
    end

    it 'should return a team with team players when team players are available for that team' do
      db.execute 'insert into teams_t values (1, "tname", "teamloc", "TN")'
      db.execute "insert into team_players_t values (1, 2, 3)"
      db.execute "insert into team_players_t values (1, 2, 5)"

      team = @team_service.get 1

      expect( team ).to_not be_nil
      expect( team ).to     be_a   Team

      expect( team.players ).to_not be_nil
      expect( team.players ).to     be_a   Array

      expect( team.players.length ).to eq 2

      expect( team.players[0].team_id   ).to eq 1
      expect( team.players[0].season    ).to eq 2
      expect( team.players[0].player_id ).to eq 3

      expect( team.players[1].team_id   ).to eq 1
      expect( team.players[1].season    ).to eq 2
      expect( team.players[1].player_id ).to eq 5
    end

    it 'should return a team without team players when team players are not available for that team' do
      db.execute 'insert into teams_t values (1, "tname", "teamloc", "TN")'

      team = @team_service.get 1

      expect( team ).to_not be_nil
      expect( team ).to     be_a   Team

      expect( team.players ).to be_nil
    end

    it 'should return a team with team players that has a player if that player is available' do
      db.execute 'insert into teams_t values (1, "tname", "teamloc", "TN")'
      db.execute "insert into team_players_t values (1, 2, 3)"
      db.execute "insert into players_t values (3, 'fname', 'lname', 123, #{Positions::Cornerback}, 64, 2, 3)"

      team = @team_service.get 1

      expect( team ).to_not be_nil
      expect( team ).to     be_a   Team

      expect( team.players ).to_not be_nil
      expect( team.players ).to     be_a   Array

      expect( team.players.length ).to eq 1

      expect( team.players[0].team_id   ).to eq 1
      expect( team.players[0].season    ).to eq 2
      expect( team.players[0].player_id ).to eq 3

      expect( team.players[0].player ).to_not be_nil

      expect( team.players[0].player.player_id       ).to eq 3
      expect( team.players[0].player.first_name      ).to eq 'fname'
      expect( team.players[0].player.last_name       ).to eq 'lname'
      expect( team.players[0].player.face            ).to eq 123
      expect( team.players[0].player.position        ).to eq Positions::Cornerback
      expect( team.players[0].player.number          ).to eq 64
      expect( team.players[0].player.freshman_season ).to eq 2
      expect( team.players[0].player.maturity        ).to eq 3
    end

    it 'should return a team with stats when stats are available for that team' do
      db.execute 'insert into teams_t values (1, "tname", "teamloc", "TN")'
      db.execute "insert into team_stats_t values (1, 2, #{Bowls::SugarBowl}, 8, 6, 2, 5, 1, 4, 3, 5, 1, 282, 268)"
      db.execute "insert into team_stats_t values (1, 3, #{Bowls::None     }, 6, 4, 5, 5, 0, 2, 1, 4, 3, 367, 314)"

      team = @team_service.get 1

      expect( team ).to_not be_nil
      expect( team ).to     be_a   Team

      expect( team.stats ).to_not be_nil
      expect( team.stats ).to     be_a   Array

      expect( team.stats.length ).to eq 2

      expect( team.stats[0].team_id        ).to eq 1
      expect( team.stats[0].season         ).to eq 2
      expect( team.stats[0].bowl_game      ).to eq Bowls::SugarBowl
      expect( team.stats[0].wins           ).to eq 8
      expect( team.stats[0].losses         ).to eq 6
      expect( team.stats[0].home_wins      ).to eq 5
      expect( team.stats[0].home_losses    ).to eq 1
      expect( team.stats[0].road_wins      ).to eq 3
      expect( team.stats[0].road_losses    ).to eq 5
      expect( team.stats[0].points_scored  ).to eq 282
      expect( team.stats[0].points_allowed ).to eq 268

      expect( team.stats[1].team_id        ).to eq 1
      expect( team.stats[1].season         ).to eq 3
      expect( team.stats[1].bowl_game      ).to eq Bowls::None
      expect( team.stats[1].wins           ).to eq 6
      expect( team.stats[1].losses         ).to eq 4
      expect( team.stats[1].home_wins      ).to eq 5
      expect( team.stats[1].home_losses    ).to eq 0
      expect( team.stats[1].road_wins      ).to eq 1
      expect( team.stats[1].road_losses    ).to eq 4
      expect( team.stats[1].points_scored  ).to eq 367
      expect( team.stats[1].points_allowed ).to eq 314
    end

    it 'should return a team without stats when stats are not available for that team' do
      db.execute 'insert into teams_t values (1, "tname", "teamloc", "TN")'

      team = @team_service.get 1

      expect( team ).to_not be_nil
      expect( team ).to     be_a   Team

      expect( team.stats ).to be_nil
    end

    it 'should return a team with offense stats when offense stats are available for that team' do
      db.execute 'insert into teams_t values (1, "tname", "teamloc", "TN")'
      db.execute "insert into team_offense_stats_t values (1, 2, #{Bowls::SugarBowl}, 314, 268, 12, 2884, 35, 287, 1083, 16)"
      db.execute "insert into team_offense_stats_t values (1, 3, #{Bowls::None     }, 285, 254, 17, 2691, 27, 303, 1142, 13)"

      team = @team_service.get 1

      expect( team ).to_not be_nil
      expect( team ).to     be_a   Team

      expect( team.offense_stats ).to_not be_nil
      expect( team.offense_stats ).to     be_a   Array

      expect( team.offense_stats.length ).to eq 2

      expect( team.offense_stats[0].team_id         ).to eq 1
      expect( team.offense_stats[0].season          ).to eq 2
      expect( team.offense_stats[0].bowl_game       ).to eq Bowls::SugarBowl
      expect( team.offense_stats[0].pass_attempts   ).to eq 314
      expect( team.offense_stats[0].completions     ).to eq 268
      expect( team.offense_stats[0].interceptions   ).to eq 12
      expect( team.offense_stats[0].pass_yards      ).to eq 2884
      expect( team.offense_stats[0].pass_touchdowns ).to eq 35
      expect( team.offense_stats[0].rush_attempts   ).to eq 287
      expect( team.offense_stats[0].rush_yards      ).to eq 1083
      expect( team.offense_stats[0].rush_touchdowns ).to eq 16

      expect( team.offense_stats[1].team_id         ).to eq 1
      expect( team.offense_stats[1].season          ).to eq 3
      expect( team.offense_stats[1].bowl_game       ).to eq Bowls::None
      expect( team.offense_stats[1].pass_attempts   ).to eq 285
      expect( team.offense_stats[1].completions     ).to eq 254
      expect( team.offense_stats[1].interceptions   ).to eq 17
      expect( team.offense_stats[1].pass_yards      ).to eq 2691
      expect( team.offense_stats[1].pass_touchdowns ).to eq 27
      expect( team.offense_stats[1].rush_attempts   ).to eq 303
      expect( team.offense_stats[1].rush_yards      ).to eq 1142
      expect( team.offense_stats[1].rush_touchdowns ).to eq 13
    end

    it 'should return a team without offense stats when offense stats are not available for that team' do
      db.execute 'insert into teams_t values (1, "tname", "teamloc", "TN")'

      team = @team_service.get 1

      expect( team ).to_not be_nil
      expect( team ).to     be_a   Team

      expect( team.offense_stats ).to be_nil
    end

    it 'should return a team with defense stats when defense stats are available for that team' do
      db.execute 'insert into teams_t values (1, "tname", "teamloc", "TN")'
      db.execute "insert into team_defense_stats_t values (1, 2, #{Bowls::SugarBowl}, 18, 7, 118, 1)"
      db.execute "insert into team_defense_stats_t values (1, 3, #{Bowls::None     }, 23, 5,  73, 0)"

      team = @team_service.get 1

      expect( team ).to_not be_nil
      expect( team ).to     be_a   Team

      expect( team.defense_stats ).to_not be_nil
      expect( team.defense_stats ).to     be_a   Array

      expect( team.defense_stats.length ).to eq 2

      expect( team.defense_stats[0].team_id           ).to eq 1
      expect( team.defense_stats[0].season            ).to eq 2
      expect( team.defense_stats[0].bowl_game         ).to eq Bowls::SugarBowl
      expect( team.defense_stats[0].sacks             ).to eq 18
      expect( team.defense_stats[0].interceptions     ).to eq 7
      expect( team.defense_stats[0].return_yards      ).to eq 118
      expect( team.defense_stats[0].return_touchdowns ).to eq 1

      expect( team.defense_stats[1].team_id           ).to eq 1
      expect( team.defense_stats[1].season            ).to eq 3
      expect( team.defense_stats[1].bowl_game         ).to eq Bowls::None
      expect( team.defense_stats[1].sacks             ).to eq 23
      expect( team.defense_stats[1].interceptions     ).to eq 5
      expect( team.defense_stats[1].return_yards      ).to eq 73
      expect( team.defense_stats[1].return_touchdowns ).to eq 0
    end

    it 'should return a team without defense stats when defense stats are not available for that team' do
      db.execute 'insert into teams_t values (1, "tname", "teamloc", "TN")'

      team = @team_service.get 1

      expect( team ).to_not be_nil
      expect( team ).to     be_a   Team

      expect( team.defense_stats ).to be_nil
    end

    it 'should return a team with kicking stats when kicking stats are available for that team' do
      db.execute 'insert into teams_t values (1, "tname", "teamloc", "TN")'
      db.execute "insert into team_kicking_stats_t values (1, 2, #{Bowls::SugarBowl}, 21, 20, 25, 18, 34, 162, 27, 141, 1, 36, 108, 2)"
      db.execute "insert into team_kicking_stats_t values (1, 3, #{Bowls::None     }, 28, 28, 24, 20, 36, 204, 25, 303, 3, 15,  94, 0)"

      team = @team_service.get 1

      expect( team ).to_not be_nil
      expect( team ).to     be_a   Team

      expect( team.kicking_stats ).to_not be_nil
      expect( team.kicking_stats ).to     be_a   Array

      expect( team.kicking_stats.length ).to eq 2

      expect( team.kicking_stats[0].team_id                ).to eq 1
      expect( team.kicking_stats[0].season                 ).to eq 2
      expect( team.kicking_stats[0].bowl_game              ).to eq Bowls::SugarBowl
      expect( team.kicking_stats[0].extra_point_attempts   ).to eq 21
      expect( team.kicking_stats[0].extra_points_made      ).to eq 20
      expect( team.kicking_stats[0].field_goal_attempts    ).to eq 25
      expect( team.kicking_stats[0].field_goals_made       ).to eq 18
      expect( team.kicking_stats[0].punts                  ).to eq 34
      expect( team.kicking_stats[0].punt_yards             ).to eq 162
      expect( team.kicking_stats[0].kick_returns           ).to eq 27
      expect( team.kicking_stats[0].kick_return_yards      ).to eq 141
      expect( team.kicking_stats[0].kick_return_touchdowns ).to eq 1
      expect( team.kicking_stats[0].punt_returns           ).to eq 36
      expect( team.kicking_stats[0].punt_return_yards      ).to eq 108
      expect( team.kicking_stats[0].punt_return_touchdowns ).to eq 2

      expect( team.kicking_stats[1].team_id                ).to eq 1
      expect( team.kicking_stats[1].season                 ).to eq 3
      expect( team.kicking_stats[1].bowl_game              ).to eq Bowls::None
      expect( team.kicking_stats[1].extra_point_attempts   ).to eq 28
      expect( team.kicking_stats[1].extra_points_made      ).to eq 28
      expect( team.kicking_stats[1].field_goal_attempts    ).to eq 24
      expect( team.kicking_stats[1].field_goals_made       ).to eq 20
      expect( team.kicking_stats[1].punts                  ).to eq 36
      expect( team.kicking_stats[1].punt_yards             ).to eq 204
      expect( team.kicking_stats[1].kick_returns           ).to eq 25
      expect( team.kicking_stats[1].kick_return_yards      ).to eq 303
      expect( team.kicking_stats[1].kick_return_touchdowns ).to eq 3
      expect( team.kicking_stats[1].punt_returns           ).to eq 15
      expect( team.kicking_stats[1].punt_return_yards      ).to eq 94
      expect( team.kicking_stats[1].punt_return_touchdowns ).to eq 0
    end

    it 'should return a team without kicking stats when kicking stats are not available for that team' do
      db.execute 'insert into teams_t values (1, "tname", "teamloc", "TN")'

      team = @team_service.get 1

      expect( team ).to_not be_nil
      expect( team ).to     be_a   Team

      expect( team.kicking_stats ).to be_nil
    end
  end

  describe '#save' do
    before :each do
      @mock_repository = double( 'Repository' )
      player_service = PlayerService.new @mock_repository
      @team_service  = TeamService.new @mock_repository, player_service
    end

    it 'should save the team' do
      team = Team.build 1

      expect( @mock_repository ).to receive( :create ).with team

      @team_service.save team
    end

    it 'should save the team accolade' do
      team          = Team.build 1
      team_accolade = TeamAccolade.build 1, 2, TeamAccolades::ConferenceChampions

      team.accolades = team_accolade

      expect( @mock_repository ).to receive( :create ).with team
      expect( @mock_repository ).to receive( :create ).with team_accolade

      @team_service.save team
    end

    it 'should save many team accolades' do
      team           = Team.build 1
      team_accolade1 = TeamAccolade.build 1, 2, TeamAccolades::ConferenceChampions
      team_accolade2 = TeamAccolade.build 1, 2, TeamAccolades::LibertyBowlChampions

      team.accolades = [team_accolade1, team_accolade2]

      expect( @mock_repository ).to receive( :create ).with team
      expect( @mock_repository ).to receive( :create ).with team_accolade1
      expect( @mock_repository ).to receive( :create ).with team_accolade2

      @team_service.save team
    end

    it 'should save the team player' do
      team        = Team.build 1
      team_player = TeamPlayer.build 1, 2, 3

      team.players = team_player

      expect( @mock_repository ).to receive( :create ).with team
      expect( @mock_repository ).to receive( :create ).with team_player

      @team_service.save team
    end

    it 'should save the team player and player ' do
      team        = Team.build 1
      team_player = TeamPlayer.build 1, 2, 3
      player      = Player.build 3

      team_player.player = player
      team.players       = team_player

      expect( @mock_repository ).to receive( :create ).with team
      expect( @mock_repository ).to receive( :create ).with team_player
      expect( @mock_repository ).to receive( :create ).with player

      @team_service.save team
    end

    it 'should save many team players' do
      team         = Team.build 1
      team_player1 = TeamPlayer.build 1, 2, 3
      team_player2 = TeamPlayer.build 1, 2, 5

      team.players = [team_player1, team_player2]

      expect( @mock_repository ).to receive( :create ).with team
      expect( @mock_repository ).to receive( :create ).with team_player1
      expect( @mock_repository ).to receive( :create ).with team_player2

      @team_service.save team
    end

    it 'should save the team stats' do
      team       = Team.build 1
      team_stats = TeamStats.build 1, 2, Bowls::OrangeBowl

      team.stats = team_stats

      expect( @mock_repository ).to receive( :create ).with team
      expect( @mock_repository ).to receive( :create ).with team_stats

      @team_service.save team
    end

    it 'should save many team stats' do
      team        = Team.build 1
      team_stats1 = TeamStats.build 1, 2, Bowls::OrangeBowl
      team_stats2 = TeamStats.build 1, 2, Bowls::NCFOChampionship

      team.stats = [team_stats1, team_stats2]

      expect( @mock_repository ).to receive( :create ).with team
      expect( @mock_repository ).to receive( :create ).with team_stats1
      expect( @mock_repository ).to receive( :create ).with team_stats2

      @team_service.save team
    end

    it 'should save the team offense stats' do
      team               = Team.build 1
      team_offense_stats = TeamOffenseStats.build 1, 2, Bowls::OrangeBowl

      team.offense_stats = team_offense_stats

      expect( @mock_repository ).to receive( :create ).with team
      expect( @mock_repository ).to receive( :create ).with team_offense_stats

      @team_service.save team
    end

    it 'should save many team offense stats' do
      team                = Team.build 1
      team_offense_stats1 = TeamOffenseStats.build 1, 2, Bowls::OrangeBowl
      team_offense_stats2 = TeamOffenseStats.build 1, 2, Bowls::NCFOChampionship

      team.offense_stats = [team_offense_stats1, team_offense_stats2]

      expect( @mock_repository ).to receive( :create ).with team
      expect( @mock_repository ).to receive( :create ).with team_offense_stats1
      expect( @mock_repository ).to receive( :create ).with team_offense_stats2

      @team_service.save team
    end

    it 'should save the team defense stats' do
      team               = Team.build 1
      team_defense_stats = TeamDefenseStats.build 1, 2, Bowls::OrangeBowl

      team.defense_stats = team_defense_stats

      expect( @mock_repository ).to receive( :create ).with team
      expect( @mock_repository ).to receive( :create ).with team_defense_stats

      @team_service.save team
    end

    it 'should save many team defense stats' do
      team                = Team.build 1
      team_defense_stats1 = TeamDefenseStats.build 1, 2, Bowls::OrangeBowl
      team_defense_stats2 = TeamDefenseStats.build 1, 2, Bowls::NCFOChampionship

      team.defense_stats = [team_defense_stats1, team_defense_stats2]

      expect( @mock_repository ).to receive( :create ).with team
      expect( @mock_repository ).to receive( :create ).with team_defense_stats1
      expect( @mock_repository ).to receive( :create ).with team_defense_stats2

      @team_service.save team
    end

    it 'should save the team kicking stats' do
      team               = Team.build 1
      team_kicking_stats = TeamKickingStats.build 1, 2, Bowls::OrangeBowl

      team.kicking_stats = team_kicking_stats

      expect( @mock_repository ).to receive( :create ).with team
      expect( @mock_repository ).to receive( :create ).with team_kicking_stats

      @team_service.save team
    end

    it 'should save many team kicking stats' do
      team                = Team.build 1
      team_kicking_stats1 = TeamKickingStats.build 1, 2, Bowls::OrangeBowl
      team_kicking_stats2 = TeamKickingStats.build 1, 2, Bowls::NCFOChampionship

      team.kicking_stats = [team_kicking_stats1, team_kicking_stats2]

      expect( @mock_repository ).to receive( :create ).with team
      expect( @mock_repository ).to receive( :create ).with team_kicking_stats1
      expect( @mock_repository ).to receive( :create ).with team_kicking_stats2

      @team_service.save team
    end
  end
end
