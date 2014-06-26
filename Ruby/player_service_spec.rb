location = File.dirname __FILE__
$: << "#{location}"

require 'sqlite3'
require 'repository'
require 'bowls'
require 'player_accolades'
require 'player_service'
require 'player'
require 'player_accolade'
require 'player_ratings'
require 'player_quarterback_ratings'
require 'player_offense_ratings'
require 'player_defense_ratings'
require 'player_kicking_ratings'
require 'player_offense_stats'
require 'player_defense_stats'
require 'player_kicking_stats'
require 'player_returns_stats'


db = SQLite3::Database.new 'ncfo_test.db'

db.results_as_hash  = true
db.type_translation = true


describe 'PlayerService' do
  describe '#get' do
    before :each do
      @repository = Repository.new db
      @player_service  = PlayerService.new @repository

      @repository.start_transaction
    end

    after :each do
      @repository.cancel_transaction
    end

    it 'should return a player with values from the database record' do
      db.execute "insert into players_t values (1, 'fname', 'lname', 123, #{Positions::Cornerback}, 64, 2, 3)"

      player = @player_service.get 1

      expect( player ).to_not be_nil
      expect( player ).to     be_a   Player

      expect( player.player_id       ).to eq 1
      expect( player.first_name      ).to eq 'fname'
      expect( player.last_name       ).to eq 'lname'
      expect( player.face            ).to eq 123
      expect( player.position        ).to eq Positions::Cornerback
      expect( player.number          ).to eq 64
      expect( player.freshman_season ).to eq 2
      expect( player.maturity        ).to eq 3
    end

    it 'should return nil if the player is not found' do
      player = @player_service.get 1

      expect( player ).to be_nil
    end

    it 'should return a player with accolades when accolades are available for that player' do
      db.execute "insert into players_t values (1, 'fname', 'lname', 123, #{Positions::Cornerback}, 64, 2, 3)"
      db.execute "insert into player_accolades_t values (1, 2, #{PlayerAccolades::Defensive_Player_of_the_Year})"
      db.execute "insert into player_accolades_t values (1, 2, #{PlayerAccolades::Most_Interceptions          })"

      player = @player_service.get 1

      expect( player ).to_not be_nil
      expect( player ).to     be_a   Player

      expect( player.accolades ).to_not be_nil
      expect( player.accolades ).to     be_a   Array

      expect( player.accolades.length ).to eq 2

      expect( player.accolades[0].player_id ).to eq 1
      expect( player.accolades[0].season    ).to eq 2
      expect( player.accolades[0].accolade  ).to eq PlayerAccolades::Defensive_Player_of_the_Year

      expect( player.accolades[1].player_id ).to eq 1
      expect( player.accolades[1].season    ).to eq 2
      expect( player.accolades[1].accolade  ).to eq PlayerAccolades::Most_Interceptions
    end

    it 'should return a player without accolades when accolades are not available for that player' do
      db.execute "insert into players_t values (1, 'fname', 'lname', 123, #{Positions::Cornerback}, 64, 2, 3)"

      player = @player_service.get 1

      expect( player ).to_not be_nil
      expect( player ).to     be_a   Player

      expect( player.accolades ).to be_nil
    end

    it 'should return a player with ratings when ratings are available for that player' do
      db.execute "insert into players_t values (1, 'fname', 'lname', 123, #{Positions::Cornerback}, 64, 2, 3)"
      db.execute 'insert into player_ratings_t values (1, 2, 3, 4, 5)'

      player = @player_service.get 1

      expect( player ).to_not be_nil
      expect( player ).to     be_a   Player

      expect( player.ratings ).to_not be_nil
      expect( player.ratings ).to     be_a   PlayerRatings

      expect( player.ratings.player_id  ).to eq 1
      expect( player.ratings.run_speed  ).to eq 2
      expect( player.ratings.rush_power ).to eq 3
      expect( player.ratings.max_speed  ).to eq 4
      expect( player.ratings.hit_power  ).to eq 5
    end

    it 'should return a player without ratings when ratings are not available for that player' do
      db.execute "insert into players_t values (1, 'fname', 'lname', 123, #{Positions::Cornerback}, 64, 2, 3)"

      player = @player_service.get 1

      expect( player ).to_not be_nil
      expect( player ).to     be_a   Player

      expect( player.ratings ).to be_nil
    end

    it 'should return a player with quarterback ratings when quarterback ratings are available for that player' do
      db.execute "insert into players_t values (1, 'fname', 'lname', 123, #{Positions::Quarterback}, 64, 2, 3)"
      db.execute 'insert into player_quarterback_ratings_t values (1, 2, 3, 4, 5)'

      player = @player_service.get 1

      expect( player ).to_not be_nil
      expect( player ).to     be_a   Player

      expect( player.quarterback_ratings ).to_not be_nil
      expect( player.quarterback_ratings ).to     be_a   PlayerQuarterbackRatings

      expect( player.quarterback_ratings.player_id        ).to eq 1
      expect( player.quarterback_ratings.pass_speed       ).to eq 2
      expect( player.quarterback_ratings.pass_control     ).to eq 3
      expect( player.quarterback_ratings.pass_accuracy    ).to eq 4
      expect( player.quarterback_ratings.avoid_pass_block ).to eq 5
    end

    it 'should return a player without quarterback ratings when quarterback ratings are not available for that player' do
      db.execute "insert into players_t values (1, 'fname', 'lname', 123, #{Positions::Quarterback}, 64, 2, 3)"

      player = @player_service.get 1

      expect( player ).to_not be_nil
      expect( player ).to     be_a   Player

      expect( player.ratings ).to be_nil
    end

    it 'should return a player without quarterback ratings when player is not a quarterback' do
      db.execute "insert into players_t values (1, 'fname', 'lname', 123, #{Positions::Cornerback}, 64, 2, 3)"
      db.execute 'insert into player_quarterback_ratings_t values (1, 2, 3, 4, 5)' # should never happen

      player = @player_service.get 1

      expect( player ).to_not be_nil
      expect( player ).to     be_a   Player

      expect( player.ratings ).to be_nil
    end

    it 'should return a player with offense ratings when offense ratings are available for that player' do
      db.execute "insert into players_t values (1, 'fname', 'lname', 123, #{Positions::WideReceiver}, 64, 2, 3)"
      db.execute 'insert into player_offense_ratings_t values (1, 2, 3)'

      player = @player_service.get 1

      expect( player ).to_not be_nil
      expect( player ).to     be_a   Player

      expect( player.offense_ratings ).to_not be_nil
      expect( player.offense_ratings ).to     be_a   PlayerOffenseRatings

      expect( player.offense_ratings.player_id    ).to eq 1
      expect( player.offense_ratings.ball_control ).to eq 2
      expect( player.offense_ratings.receiving    ).to eq 3
    end

    it 'should return a player without offense ratings when offense ratings are not available for that player' do
      db.execute "insert into players_t values (1, 'fname', 'lname', 123, #{Positions::WideReceiver}, 64, 2, 3)"

      player = @player_service.get 1

      expect( player ).to_not be_nil
      expect( player ).to     be_a   Player

      expect( player.offense_ratings ).to be_nil
    end

    it 'should return a player without offense ratings when player is not a non-quarterback non-linesman offense player' do
      db.execute "insert into players_t values (1, 'fname', 'lname', 123, #{Positions::Cornerback}, 64, 2, 3)"
      db.execute 'insert into player_offense_ratings_t values (1, 2, 3)' # should never happen

      player = @player_service.get 1

      expect( player ).to_not be_nil
      expect( player ).to     be_a   Player

      expect( player.offense_ratings ).to be_nil
    end

    it 'should return a player with defense ratings when defense ratings are available for that player' do
      db.execute "insert into players_t values (1, 'fname', 'lname', 123, #{Positions::Cornerback}, 64, 2, 3)"
      db.execute 'insert into player_defense_ratings_t values (1, 2, 3)'

      player = @player_service.get 1

      expect( player ).to_not be_nil
      expect( player ).to     be_a   Player

      expect( player.defense_ratings ).to_not be_nil
      expect( player.defense_ratings ).to     be_a   PlayerDefenseRatings

      expect( player.defense_ratings.player_id     ).to eq 1
      expect( player.defense_ratings.interceptions ).to eq 2
      expect( player.defense_ratings.quickness     ).to eq 3
    end

    it 'should return a player without defense ratings when defense ratings are not available for that player' do
      db.execute "insert into players_t values (1, 'fname', 'lname', 123, #{Positions::Cornerback}, 64, 2, 3)"

      player = @player_service.get 1

      expect( player ).to_not be_nil
      expect( player ).to     be_a   Player

      expect( player.defense_ratings ).to be_nil
    end

    it 'should return a player without defense ratings when player is not a defense player' do
      db.execute "insert into players_t values (1, 'fname', 'lname', 123, #{Positions::Quarterback}, 64, 2, 3)"
      db.execute 'insert into player_defense_ratings_t values (1, 2, 3)' # should never happen

      player = @player_service.get 1

      expect( player ).to_not be_nil
      expect( player ).to     be_a   Player

      expect( player.defense_ratings ).to be_nil
    end

    it 'should return a player with kicking ratings when kicking ratings are available for that player' do
      db.execute "insert into players_t values (1, 'fname', 'lname', 123, #{Positions::Punter}, 64, 2, 3)"
      db.execute 'insert into player_kicking_ratings_t values (1, 2, 3)'

      player = @player_service.get 1

      expect( player ).to_not be_nil
      expect( player ).to     be_a   Player

      expect( player.kicking_ratings ).to_not be_nil
      expect( player.kicking_ratings ).to     be_a   PlayerKickingRatings

      expect( player.kicking_ratings.player_id        ).to eq 1
      expect( player.kicking_ratings.kicking_ability  ).to eq 2
      expect( player.kicking_ratings.avoid_kick_block ).to eq 3
    end

    it 'should return a player without kicking ratings when kicking ratings are not available for that player' do
      db.execute "insert into players_t values (1, 'fname', 'lname', 123, #{Positions::Punter}, 64, 2, 3)"

      player = @player_service.get 1

      expect( player ).to_not be_nil
      expect( player ).to     be_a   Player

      expect( player.kicking_ratings ).to be_nil
    end

    it 'should return a player without kicking ratings when player is not a kicker' do
      db.execute "insert into players_t values (1, 'fname', 'lname', 123, #{Positions::Safety}, 64, 2, 3)"
      db.execute 'insert into player_kicking_ratings_t values (1, 2, 3)' # should never happen

      player = @player_service.get 1

      expect( player ).to_not be_nil
      expect( player ).to     be_a   Player

      expect( player.kicking_ratings ).to be_nil
    end

    it 'should return a player with offense stats when offense stats are available for that player' do
      db.execute "insert into players_t values (1, 'fname', 'lname', 123, #{Positions::Quarterback}, 64, 2, 3)"
      db.execute "insert into player_offense_stats_t values (1, 2, #{Bowls::RoseBowl},   314, 268, 12, 2884, 35, 287, 1083, 16, 83, 1146, 12)"
      db.execute "insert into player_offense_stats_t values (1, 2, #{Bowls::FiestaBowl}, 297, 221,  9, 2385, 27, 216,  992, 12, 57,  947,  8)"

      player = @player_service.get 1

      expect( player ).to_not be_nil
      expect( player ).to     be_a   Player

      expect( player.offense_stats ).to_not be_nil
      expect( player.offense_stats ).to     be_a   Array

      expect( player.offense_stats.length ).to eq 2

      expect( player.offense_stats[0].player_id            ).to eq 1
      expect( player.offense_stats[0].season               ).to eq 2
      expect( player.offense_stats[0].bowl_game            ).to eq Bowls::RoseBowl
      expect( player.offense_stats[0].pass_attempts        ).to eq 314
      expect( player.offense_stats[0].completions          ).to eq 268
      expect( player.offense_stats[0].interceptions        ).to eq 12
      expect( player.offense_stats[0].pass_yards           ).to eq 2884
      expect( player.offense_stats[0].pass_touchdowns      ).to eq 35
      expect( player.offense_stats[0].rush_attempts        ).to eq 287
      expect( player.offense_stats[0].rush_yards           ).to eq 1083
      expect( player.offense_stats[0].rush_touchdowns      ).to eq 16
      expect( player.offense_stats[0].receptions           ).to eq 83
      expect( player.offense_stats[0].receiving_yards      ).to eq 1146
      expect( player.offense_stats[0].receiving_touchdowns ).to eq 12

      expect( player.offense_stats[1].player_id            ).to eq 1
      expect( player.offense_stats[1].season               ).to eq 2
      expect( player.offense_stats[1].bowl_game            ).to eq Bowls::FiestaBowl
      expect( player.offense_stats[1].pass_attempts        ).to eq 297
      expect( player.offense_stats[1].completions          ).to eq 221
      expect( player.offense_stats[1].interceptions        ).to eq 9
      expect( player.offense_stats[1].pass_yards           ).to eq 2385
      expect( player.offense_stats[1].pass_touchdowns      ).to eq 27
      expect( player.offense_stats[1].rush_attempts        ).to eq 216
      expect( player.offense_stats[1].rush_yards           ).to eq 992
      expect( player.offense_stats[1].rush_touchdowns      ).to eq 12
      expect( player.offense_stats[1].receptions           ).to eq 57
      expect( player.offense_stats[1].receiving_yards      ).to eq 947
      expect( player.offense_stats[1].receiving_touchdowns ).to eq 8
    end

    it 'should return a player without offense stats when offense stats are not available for that player' do
      db.execute "insert into players_t values (1, 'fname', 'lname', 123, #{Positions::Quarterback}, 64, 2, 3)"

      player = @player_service.get 1

      expect( player ).to_not be_nil
      expect( player ).to     be_a   Player

      expect( player.offense_stats ).to be_nil
    end

    it 'should return a player with defense stats when defense stats are available for that player' do
      db.execute "insert into players_t values (1, 'fname', 'lname', 123, #{Positions::Quarterback}, 64, 2, 3)"
      db.execute "insert into player_defense_stats_t values (1, 2, #{Bowls::RoseBowl},   18, 5, 116, 1)"
      db.execute "insert into player_defense_stats_t values (1, 2, #{Bowls::FiestaBowl}, 21, 7, 204, 0)"

      player = @player_service.get 1

      expect( player ).to_not be_nil
      expect( player ).to     be_a   Player

      expect( player.defense_stats ).to_not be_nil
      expect( player.defense_stats ).to     be_a   Array

      expect( player.defense_stats.length ).to eq 2

      expect( player.defense_stats[0].player_id         ).to eq 1
      expect( player.defense_stats[0].season            ).to eq 2
      expect( player.defense_stats[0].bowl_game         ).to eq Bowls::RoseBowl
      expect( player.defense_stats[0].sacks             ).to eq 18
      expect( player.defense_stats[0].interceptions     ).to eq 5
      expect( player.defense_stats[0].return_yards      ).to eq 116
      expect( player.defense_stats[0].return_touchdowns ).to eq 1

      expect( player.defense_stats[1].player_id         ).to eq 1
      expect( player.defense_stats[1].season            ).to eq 2
      expect( player.defense_stats[1].bowl_game         ).to eq Bowls::FiestaBowl
      expect( player.defense_stats[1].sacks             ).to eq 21
      expect( player.defense_stats[1].interceptions     ).to eq 7
      expect( player.defense_stats[1].return_yards      ).to eq 204
      expect( player.defense_stats[1].return_touchdowns ).to eq 0
    end

    it 'should return a player without defense stats when defense stats are not available for that player' do
      db.execute "insert into players_t values (1, 'fname', 'lname', 123, #{Positions::Quarterback}, 64, 2, 3)"

      player = @player_service.get 1

      expect( player ).to_not be_nil
      expect( player ).to     be_a   Player

      expect( player.defense_stats ).to be_nil
    end

    it 'should return a player with kicking stats when kicking stats are available for that player' do
      db.execute "insert into players_t values (1, 'fname', 'lname', 123, #{Positions::Quarterback}, 64, 2, 3)"
      db.execute "insert into player_kicking_stats_t values (1, 2, #{Bowls::RoseBowl},   23, 22, 26, 21, 31, 494)"
      db.execute "insert into player_kicking_stats_t values (1, 2, #{Bowls::FiestaBowl}, 28, 28, 33, 29, 24, 318)"

      player = @player_service.get 1

      expect( player ).to_not be_nil
      expect( player ).to     be_a   Player

      expect( player.kicking_stats ).to_not be_nil
      expect( player.kicking_stats ).to     be_a   Array

      expect( player.kicking_stats.length ).to eq 2

      expect( player.kicking_stats[0].player_id            ).to eq 1
      expect( player.kicking_stats[0].season               ).to eq 2
      expect( player.kicking_stats[0].bowl_game            ).to eq Bowls::RoseBowl
      expect( player.kicking_stats[0].extra_point_attempts ).to eq 23
      expect( player.kicking_stats[0].extra_points_made    ).to eq 22
      expect( player.kicking_stats[0].field_goal_attempts  ).to eq 26
      expect( player.kicking_stats[0].field_goals_made     ).to eq 21
      expect( player.kicking_stats[0].punts                ).to eq 31
      expect( player.kicking_stats[0].punt_yards           ).to eq 494

      expect( player.kicking_stats[1].player_id            ).to eq 1
      expect( player.kicking_stats[1].season               ).to eq 2
      expect( player.kicking_stats[1].bowl_game            ).to eq Bowls::FiestaBowl
      expect( player.kicking_stats[1].extra_point_attempts ).to eq 28
      expect( player.kicking_stats[1].extra_points_made    ).to eq 28
      expect( player.kicking_stats[1].field_goal_attempts  ).to eq 33
      expect( player.kicking_stats[1].field_goals_made     ).to eq 29
      expect( player.kicking_stats[1].punts                ).to eq 24
      expect( player.kicking_stats[1].punt_yards           ).to eq 318
    end

    it 'should return a player without kicking stats when kicking stats are not available for that player' do
      db.execute "insert into players_t values (1, 'fname', 'lname', 123, #{Positions::Quarterback}, 64, 2, 3)"

      player = @player_service.get 1

      expect( player ).to_not be_nil
      expect( player ).to     be_a   Player

      expect( player.kicking_stats ).to be_nil
    end

    it 'should return a player with returns stats when returns stats are available for that player' do
      db.execute "insert into players_t values (1, 'fname', 'lname', 123, #{Positions::Quarterback}, 64, 2, 3)"
      db.execute "insert into player_returns_stats_t values (1, 2, #{Bowls::RoseBowl},   34, 583, 2, 27, 255, 1)"
      db.execute "insert into player_returns_stats_t values (1, 2, #{Bowls::FiestaBowl}, 28, 496, 1, 29, 451, 3)"

      player = @player_service.get 1

      expect( player ).to_not be_nil
      expect( player ).to     be_a   Player

      expect( player.returns_stats ).to_not be_nil
      expect( player.returns_stats ).to     be_a   Array

      expect( player.returns_stats.length ).to eq 2

      expect( player.returns_stats[0].player_id              ).to eq 1
      expect( player.returns_stats[0].season                 ).to eq 2
      expect( player.returns_stats[0].bowl_game              ).to eq Bowls::RoseBowl
      expect( player.returns_stats[0].kick_returns           ).to eq 34
      expect( player.returns_stats[0].kick_return_yards      ).to eq 583
      expect( player.returns_stats[0].kick_return_touchdowns ).to eq 2
      expect( player.returns_stats[0].punt_returns           ).to eq 27
      expect( player.returns_stats[0].punt_return_yards      ).to eq 255
      expect( player.returns_stats[0].punt_return_touchdowns ).to eq 1

      expect( player.returns_stats[1].player_id              ).to eq 1
      expect( player.returns_stats[1].season                 ).to eq 2
      expect( player.returns_stats[1].bowl_game              ).to eq Bowls::FiestaBowl
      expect( player.returns_stats[1].kick_returns           ).to eq 28
      expect( player.returns_stats[1].kick_return_yards      ).to eq 496
      expect( player.returns_stats[1].kick_return_touchdowns ).to eq 1
      expect( player.returns_stats[1].punt_returns           ).to eq 29
      expect( player.returns_stats[1].punt_return_yards      ).to eq 451
      expect( player.returns_stats[1].punt_return_touchdowns ).to eq 3
    end

    it 'should return a player without returns stats when returns stats are not available for that player' do
      db.execute "insert into players_t values (1, 'fname', 'lname', 123, #{Positions::Quarterback}, 64, 2, 3)"

      player = @player_service.get 1

      expect( player ).to_not be_nil
      expect( player ).to     be_a   Player

      expect( player.returns_stats ).to be_nil
    end

  # returns_stats

  end

  describe '#save' do
    before :each do
      @mock_repository = double( 'Repository' )
      @player_service  = PlayerService.new @mock_repository
    end

    it 'should save the player' do
      player = Player.build 1

      expect( @mock_repository ).to receive( :create ).with player

      @player_service.save player
    end

    it 'should save the player accolade' do
      player          = Player.build 1
      player_accolade = PlayerAccolade.build 1, 2, PlayerAccolades::All_American

      player.accolades = player_accolade

      expect( @mock_repository ).to receive( :create ).with player
      expect( @mock_repository ).to receive( :create ).with player_accolade

      @player_service.save player
    end

    it 'should save many player accolades' do
      player           = Player.build 1
      player_accolade1 = PlayerAccolade.build 1, 2, PlayerAccolades::All_American
      player_accolade2 = PlayerAccolade.build 1, 2, PlayerAccolades::South_Passing_Champion

      player.accolades = [player_accolade1, player_accolade2]

      expect( @mock_repository ).to receive( :create ).with player
      expect( @mock_repository ).to receive( :create ).with player_accolade1
      expect( @mock_repository ).to receive( :create ).with player_accolade2

      @player_service.save player
    end

    it 'should save the player ratings' do
      player         = Player.build 1
      player_ratings = PlayerRatings.build 1

      player.ratings = player_ratings

      expect( @mock_repository ).to receive( :create ).with player
      expect( @mock_repository ).to receive( :create ).with player_ratings

      @player_service.save player
    end

    it 'should save the player quarterback ratings' do
      player                     = Player.build 1
      player_quarterback_ratings = PlayerQuarterbackRatings.build 1

      player.quarterback_ratings = player_quarterback_ratings

      expect( @mock_repository ).to receive( :create ).with player
      expect( @mock_repository ).to receive( :create ).with player_quarterback_ratings

      @player_service.save player
    end

    it 'should save the player offense ratings' do
      player                 = Player.build 1
      player_offense_ratings = PlayerOffenseRatings.build 1

      player.offense_ratings = player_offense_ratings

      expect( @mock_repository ).to receive( :create ).with player
      expect( @mock_repository ).to receive( :create ).with player_offense_ratings

      @player_service.save player
    end

    it 'should save the player defense ratings' do
      player                 = Player.build 1
      player_defense_ratings = PlayerDefenseRatings.build 1

      player.defense_ratings = player_defense_ratings

      expect( @mock_repository ).to receive( :create ).with player
      expect( @mock_repository ).to receive( :create ).with player_defense_ratings

      @player_service.save player
    end

    it 'should save the player kicking ratings' do
      player                 = Player.build 1
      player_kicking_ratings = PlayerKickingRatings.build 1

      player.kicking_ratings = player_kicking_ratings

      expect( @mock_repository ).to receive( :create ).with player
      expect( @mock_repository ).to receive( :create ).with player_kicking_ratings

      @player_service.save player
    end

    it 'should save the player offense stats' do
      player               = Player.build 1
      player_offense_stats = PlayerOffenseStats.build 1, 2, Bowls::RoseBowl

      player.offense_stats = player_offense_stats

      expect( @mock_repository ).to receive( :create ).with player
      expect( @mock_repository ).to receive( :create ).with player_offense_stats

      @player_service.save player
    end

    it 'should save many player offense stats' do
      player                = Player.build 1
      player_offense_stats1 = PlayerOffenseStats.build 1, 2, Bowls::RoseBowl
      player_offense_stats2 = PlayerOffenseStats.build 1, 3, Bowls::RoseBowl

      player.offense_stats = [player_offense_stats1, player_offense_stats2]

      expect( @mock_repository ).to receive( :create ).with player
      expect( @mock_repository ).to receive( :create ).with player_offense_stats1
      expect( @mock_repository ).to receive( :create ).with player_offense_stats2

      @player_service.save player
    end

    it 'should save the player defense stats' do
      player               = Player.build 1
      player_defense_stats = PlayerDefenseStats.build 1, 2, Bowls::RoseBowl

      player.defense_stats = player_defense_stats

      expect( @mock_repository ).to receive( :create ).with player
      expect( @mock_repository ).to receive( :create ).with player_defense_stats

      @player_service.save player
    end

    it 'should save many player defense stats' do
      player                = Player.build 1
      player_defense_stats1 = PlayerDefenseStats.build 1, 2, Bowls::RoseBowl
      player_defense_stats2 = PlayerDefenseStats.build 1, 3, Bowls::RoseBowl

      player.defense_stats = [player_defense_stats1, player_defense_stats2]

      expect( @mock_repository ).to receive( :create ).with player
      expect( @mock_repository ).to receive( :create ).with player_defense_stats1
      expect( @mock_repository ).to receive( :create ).with player_defense_stats2

      @player_service.save player
    end

    it 'should save the player kicking stats' do
      player               = Player.build 1
      player_kicking_stats = PlayerKickingStats.build 1, 2, Bowls::RoseBowl

      player.kicking_stats = player_kicking_stats

      expect( @mock_repository ).to receive( :create ).with player
      expect( @mock_repository ).to receive( :create ).with player_kicking_stats

      @player_service.save player
    end

    it 'should save many player kicking stats' do
      player                = Player.build 1
      player_kicking_stats1 = PlayerKickingStats.build 1, 2, Bowls::RoseBowl
      player_kicking_stats2 = PlayerKickingStats.build 1, 3, Bowls::RoseBowl

      player.kicking_stats = [player_kicking_stats1, player_kicking_stats2]

      expect( @mock_repository ).to receive( :create ).with player
      expect( @mock_repository ).to receive( :create ).with player_kicking_stats1
      expect( @mock_repository ).to receive( :create ).with player_kicking_stats2

      @player_service.save player
    end

    it 'should save the player returns stats' do
      player               = Player.build 1
      player_returns_stats = PlayerReturnsStats.build 1, 2, Bowls::RoseBowl

      player.returns_stats = player_returns_stats

      expect( @mock_repository ).to receive( :create ).with player
      expect( @mock_repository ).to receive( :create ).with player_returns_stats

      @player_service.save player
    end

    it 'should save many player returns stats' do
      player                = Player.build 1
      player_returns_stats1 = PlayerReturnsStats.build 1, 2, Bowls::RoseBowl
      player_returns_stats2 = PlayerReturnsStats.build 1, 3, Bowls::RoseBowl

      player.returns_stats = [player_returns_stats1, player_returns_stats2]

      expect( @mock_repository ).to receive( :create ).with player
      expect( @mock_repository ).to receive( :create ).with player_returns_stats1
      expect( @mock_repository ).to receive( :create ).with player_returns_stats2

      @player_service.save player
    end
  end
end
