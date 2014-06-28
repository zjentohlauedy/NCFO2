location = File.dirname __FILE__
$: << "#{location}"

require 'sqlite3'
require 'bowls'
require 'repository'
require 'player'
require 'team_stats'


db = SQLite3::Database.new 'ncfo_test.db'

db.results_as_hash  = true
db.type_translation = true


describe 'Repository' do
  before :each do
    @repository = Repository.new db

    @repository.start_transaction
  end

  after :each do
    @repository.cancel_transaction
  end

  describe '#start_transaction' do
    it 'should open a new database transaction' do
      mock_db    = double('SQLite3::Database')
      repository = Repository.new mock_db

      expect( mock_db ).to receive :transaction

      repository.start_transaction
    end
  end

  describe '#cancel_transaction' do
    it 'should rollback the open transaction' do
      mock_db    = double('SQLite3::Database')
      repository = Repository.new mock_db

      expect( mock_db ).to receive :rollback

      repository.cancel_transaction
    end
  end

  describe '#end_transaction' do
    it 'should commit the open transaction' do
      mock_db    = double('SQLite3::Database')
      repository = Repository.new mock_db

      expect( mock_db ).to receive :commit

      repository.end_transaction
    end
  end

  describe '#create' do
    it 'should store the object in the database' do
      player     = Player.build 1

      @repository.create player

      actual = db.execute 'select * from players_t where player_id = 1'

      expect( actual.length                  ).to eq 1
      expect( actual[0][ 'Player_Id'       ] ).to eq player.player_id
      expect( actual[0][ 'First_Name'      ] ).to eq player.first_name
      expect( actual[0][ 'Last_Name'       ] ).to eq player.last_name
      expect( actual[0][ 'Face'            ] ).to eq player.face
      expect( actual[0][ 'Position'        ] ).to eq player.position
      expect( actual[0][ 'Number'          ] ).to eq player.number
      expect( actual[0][ 'Freshman_Season' ] ).to eq player.freshman_season
      expect( actual[0][ 'Maturity'        ] ).to eq player.maturity
    end

    it 'should raise error if provided object is not Persistable' do
      expect { @repository.create 1 }.to raise_error 'object must be persistable'
    end
  end

  describe '#custom_create' do
    it 'should execute the provided insert query' do
      @repository.custom_create 'insert into conferences_t (conference_id) values (:id)', { id: 12 }

      actual = db.execute 'select * from conferences_t where conference_id = 12'

      expect( actual.length                ).to eq 1
      expect( actual[0][ 'Conference_Id' ] ).to eq 12
      expect( actual[0][ 'Name'          ] ).to be_nil
    end

    it 'should execute the provided insert query without parameters' do
      @repository.custom_create 'insert into conferences_t (conference_id) values (5)'

      actual = db.execute 'select * from conferences_t where conference_id = 5'

      expect( actual.length                ).to eq 1
      expect( actual[0][ 'Conference_Id' ] ).to eq 5
      expect( actual[0][ 'Name'          ] ).to be_nil
    end
  end

  describe '#read' do
    it 'should return the object from the database matching the provided key' do
      team_stats = TeamStats.new

      db.execute "insert into team_stats_t values ( 1, 2, #{Bowls::RoseBowl}, 8, 2, 5, 0, 3, 2, 341, 262 )"

      team_stats.team_id   = 1
      team_stats.season    = 2
      team_stats.bowl_game = Bowls::RoseBowl

      actual = @repository.read team_stats

      expect( actual.team_id        ).to eq 1
      expect( actual.season         ).to eq 2
      expect( actual.bowl_game      ).to eq Bowls::RoseBowl
      expect( actual.wins           ).to eq 8
      expect( actual.losses         ).to eq 2
      expect( actual.home_wins      ).to eq 5
      expect( actual.home_losses    ).to eq 0
      expect( actual.road_wins      ).to eq 3
      expect( actual.road_losses    ).to eq 2
      expect( actual.points_scored  ).to eq 341
      expect( actual.points_allowed ).to eq 262
    end

    it 'should return nil if the database record is not found' do
      team_stats = TeamStats.new

      team_stats.team_id   = 1
      team_stats.season    = 2
      team_stats.bowl_game = Bowls::RoseBowl

      actual = @repository.read team_stats

      expect( actual ).to be_nil
    end

    it 'should raise error if provided object is not Persistable' do
      expect { @repository.read 1 }.to raise_error 'object must be persistable'
    end
  end

  describe '#custom_read' do
    it 'should return the results of the provided select query as a hash' do
      db.execute 'insert into team_players_t values ( 8, 2, 201 )'

      result = @repository.custom_read 'select * from team_players_t where season = :season', { season: 2 }

      expect( result.length ).to eq 1
      expect( result[0][ :team_id   ] ).to eq 8
      expect( result[0][ :season    ] ).to eq 2
      expect( result[0][ :player_id ] ).to eq 201
    end

    it 'should return the results of the provided select query as a hash' do
      db.execute 'insert into team_players_t values ( 8, 2, 201 )'

      result = @repository.custom_read 'select * from team_players_t where player_id = 201'

      expect( result.length ).to eq 1
      expect( result[0][ :team_id   ] ).to eq 8
      expect( result[0][ :season    ] ).to eq 2
      expect( result[0][ :player_id ] ).to eq 201
    end
  end

  describe '#update' do
    it 'should update the record in the database with the provided object values' do
      team_stats = TeamStats.build 1, 2, Bowls::LibertyBowl

      db.execute "insert into team_stats_t values ( 1, 2, #{Bowls::LibertyBowl}, 8, 2, 5, 0, 3, 2, 341, 262 )"

      @repository.update team_stats

      actual = db.execute "select * from team_stats_t where team_id = 1 and season = 2 and bowl_game = #{Bowls::LibertyBowl}"

      expect( actual[0][ 'Team_Id'        ] ).to eq team_stats.team_id
      expect( actual[0][ 'Season'         ] ).to eq team_stats.season
      expect( actual[0][ 'Bowl_Game'      ] ).to eq team_stats.bowl_game
      expect( actual[0][ 'Wins'           ] ).to eq team_stats.wins
      expect( actual[0][ 'Losses'         ] ).to eq team_stats.losses
      expect( actual[0][ 'Home_Wins'      ] ).to eq team_stats.home_wins
      expect( actual[0][ 'Home_Losses'    ] ).to eq team_stats.home_losses
      expect( actual[0][ 'Road_Wins'      ] ).to eq team_stats.road_wins
      expect( actual[0][ 'Road_Losses'    ] ).to eq team_stats.road_losses
      expect( actual[0][ 'Points_Scored'  ] ).to eq team_stats.points_scored
      expect( actual[0][ 'Points_Allowed' ] ).to eq team_stats.points_allowed
    end

    it 'should raise error if provided object is not Persistable' do
      expect { @repository.update 1 }.to raise_error 'object must be persistable'
    end
  end

  describe '#custom_update' do
    it 'should execute the provided update query' do
      db.execute "insert into conference_stats_t values ( 1, 2, #{Bowls::CottonBowl}, 8, 2, 5, 0, 3, 2, 341, 262 )"

      @repository.custom_update 'update conference_stats_t set wins = :wins, losses = :losses where conference_id = :conference_id', { conference_id: 1, wins: 7, losses: 3 }

      actual = db.execute "select * from conference_stats_t where conference_id = 1 and season = 2 and bowl_game = #{Bowls::CottonBowl}"

      expect( actual[0][ 'Wins'   ] ).to eq 7
      expect( actual[0][ 'Losses' ] ).to eq 3
    end

    it 'should execute the provided update query without parameters' do
      db.execute "insert into conference_stats_t values ( 1, 2, #{Bowls::CottonBowl}, 8, 2, 5, 0, 3, 2, 341, 262 )"

      @repository.custom_update 'update conference_stats_t set points_scored = 347, points_allowed = 282 where season = 2'

      actual = db.execute "select * from conference_stats_t where conference_id = 1 and season = 2 and bowl_game = #{Bowls::CottonBowl}"

      expect( actual[0][ 'Points_Scored'  ] ).to eq 347
      expect( actual[0][ 'Points_Allowed' ] ).to eq 282
    end
  end

  describe '#delete' do
    it 'remove the database record matching the key of the provided object' do
      player     = Player.new 1

      db.execute 'insert into players_t values ( 1, "first_name", "last_name", 123, 5, 56, 1, 3 )'

      @repository.delete player

      actual = db.execute 'select * from players_t where player_id = 1'

      expect( actual.length ).to eq 0
    end

    it 'should raise error if provided object is not Persistable' do
      expect { @repository.delete 1 }.to raise_error 'object must be persistable'
    end
  end

  describe '#custom_delete' do
    it 'should execute the provided delete query' do
      db.execute 'insert into teams_t values( 1, "team_name", "team home", "tn" )'

      @repository.custom_delete 'delete from teams_t where name = :name', { name: 'team_name' }

      actual = db.execute 'select * from teams_t where team_id = 1'

      expect( actual.length ).to eq 0
    end

    it 'should execute the provided delete query without parameters' do
      db.execute 'insert into teams_t values( 1, "team_name", "team home", "tn" )'

      @repository.custom_delete 'delete from teams_t'

      actual = db.execute 'select * from teams_t where team_id = 1'

      expect( actual.length ).to eq 0
    end
  end
end
