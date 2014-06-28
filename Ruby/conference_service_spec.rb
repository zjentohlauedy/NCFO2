location = File.dirname __FILE__
$: << "#{location}"

require 'sqlite3'
require 'repository'
require 'conference_service'
require 'team_service'
require 'team'
require 'bowls'
require 'conference'
require 'conference_accolade'
require 'conference_accolades'
require 'conference_stats'
require 'conference_team'


db = SQLite3::Database.new 'ncfo_test.db'

db.results_as_hash  = true
db.type_translation = true


describe 'ConferenceService' do
  describe '#get' do
    before :each do
      @repository         = Repository.new db
      team_service        = TeamService.new @repository, nil
      @conference_service = ConferenceService.new @repository, team_service

      @repository.start_transaction
    end

    after :each do
      @repository.cancel_transaction
    end

    it 'should return a conference with values from the database record' do
      db.execute 'insert into conferences_t values (1, "confname")'

      conference = @conference_service.get 1

      expect( conference ).to_not be_nil
      expect( conference ).to     be_a   Conference

      expect( conference.conference_id ).to eq 1
      expect( conference.name          ).to eq 'confname'
    end

    it 'should return nil if the conference is not found' do
      conference = @conference_service.get 1

      expect( conference ).to be_nil
    end

    it 'should return a conference with accolades when accolades are available for that conference' do
      db.execute 'insert into conferences_t values (1, "confname")'
      db.execute "insert into conference_accolades_t values (1, 2, #{ConferenceAccolades::CottonBowlChampions})"
      db.execute "insert into conference_accolades_t values (1, 2, #{ConferenceAccolades::SugarBowlChampions })"

      conference = @conference_service.get 1

      expect( conference ).to_not be_nil
      expect( conference ).to     be_a   Conference

      expect( conference.accolades ).to_not be_nil
      expect( conference.accolades ).to     be_a   Array

      expect( conference.accolades.length ).to eq 2

      expect( conference.accolades[0].conference_id ).to eq 1
      expect( conference.accolades[0].season        ).to eq 2
      expect( conference.accolades[0].accolade      ).to eq ConferenceAccolades::CottonBowlChampions

      expect( conference.accolades[1].conference_id ).to eq 1
      expect( conference.accolades[1].season        ).to eq 2
      expect( conference.accolades[1].accolade      ).to eq ConferenceAccolades::SugarBowlChampions
    end

    it 'should return a conference without accolades when accolades are not available for that conference' do
      db.execute 'insert into conferences_t values (1, "confname")'

      conference = @conference_service.get 1

      expect( conference ).to_not be_nil
      expect( conference ).to     be_a   Conference

      expect( conference.accolades ).to be_nil
    end

    it 'should return a conference with stats when stats are available for that conference' do
      db.execute 'insert into conferences_t values (1, "confname")'
      db.execute "insert into conference_stats_t values (1, 2, #{Bowls::SugarBowl}, 8, 6, 5, 1, 3, 5, 282, 268)"
      db.execute "insert into conference_stats_t values (1, 3, #{Bowls::None     }, 6, 4, 5, 0, 1, 4, 367, 314)"

      conference = @conference_service.get 1

      expect( conference ).to_not be_nil
      expect( conference ).to     be_a   Conference

      expect( conference.stats ).to_not be_nil
      expect( conference.stats ).to     be_a   Array

      expect( conference.stats.length ).to eq 2

      expect( conference.stats[0].conference_id  ).to eq 1
      expect( conference.stats[0].season         ).to eq 2
      expect( conference.stats[0].bowl_game      ).to eq Bowls::SugarBowl
      expect( conference.stats[0].wins           ).to eq 8
      expect( conference.stats[0].losses         ).to eq 6
      expect( conference.stats[0].home_wins      ).to eq 5
      expect( conference.stats[0].home_losses    ).to eq 1
      expect( conference.stats[0].road_wins      ).to eq 3
      expect( conference.stats[0].road_losses    ).to eq 5
      expect( conference.stats[0].points_scored  ).to eq 282
      expect( conference.stats[0].points_allowed ).to eq 268

      expect( conference.stats[1].conference_id  ).to eq 1
      expect( conference.stats[1].season         ).to eq 3
      expect( conference.stats[1].bowl_game      ).to eq Bowls::None
      expect( conference.stats[1].wins           ).to eq 6
      expect( conference.stats[1].losses         ).to eq 4
      expect( conference.stats[1].home_wins      ).to eq 5
      expect( conference.stats[1].home_losses    ).to eq 0
      expect( conference.stats[1].road_wins      ).to eq 1
      expect( conference.stats[1].road_losses    ).to eq 4
      expect( conference.stats[1].points_scored  ).to eq 367
      expect( conference.stats[1].points_allowed ).to eq 314
    end

    it 'should return a conference without stats when stats are not available for that conference' do
      db.execute 'insert into conferences_t values (1, "confname")'

      conference = @conference_service.get 1

      expect( conference ).to_not be_nil
      expect( conference ).to     be_a   Conference

      expect( conference.stats ).to be_nil
    end

    it 'should return a conference with conference teams when conference teams are available for that conference' do
      db.execute 'insert into conferences_t values (1, "confname")'
      db.execute "insert into conference_teams_t values (1, 3)"
      db.execute "insert into conference_teams_t values (1, 5)"

      conference = @conference_service.get 1

      expect( conference ).to_not be_nil
      expect( conference ).to     be_a   Conference

      expect( conference.teams ).to_not be_nil
      expect( conference.teams ).to     be_a   Array

      expect( conference.teams.length ).to eq 2

      expect( conference.teams[0].conference_id ).to eq 1
      expect( conference.teams[0].team_id       ).to eq 3

      expect( conference.teams[1].conference_id ).to eq 1
      expect( conference.teams[1].team_id       ).to eq 5
    end

    it 'should return a conference without conference teams when conference teams are not available for that conference' do
      db.execute 'insert into conferences_t values (1, "confname")'

      conference = @conference_service.get 1

      expect( conference ).to_not be_nil
      expect( conference ).to     be_a   Conference

      expect( conference.teams ).to be_nil
    end

    it 'should return a conference with conference teams that has a team if that team is available' do
      db.execute 'insert into conferences_t values (1, "confname")'
      db.execute "insert into conference_teams_t values (1, 3)"
      db.execute "insert into teams_t values (3, 'tname', 'loc', 'TN')"

      conference = @conference_service.get 1

      expect( conference ).to_not be_nil
      expect( conference ).to     be_a   Conference

      expect( conference.teams ).to_not be_nil
      expect( conference.teams ).to     be_a   Array

      expect( conference.teams.length ).to eq 1

      expect( conference.teams[0].conference_id ).to eq 1
      expect( conference.teams[0].team_id       ).to eq 3

      expect( conference.teams[0].team ).to_not be_nil

      expect( conference.teams[0].team.team_id      ).to eq 3
      expect( conference.teams[0].team.name         ).to eq 'tname'
      expect( conference.teams[0].team.location     ).to eq 'loc'
      expect( conference.teams[0].team.abbreviation ).to eq 'TN'
    end
  end

  describe '#save' do
    before :each do
      @mock_repository    = double( 'Repository' )
      team_service        = TeamService.new @mock_repository, nil
      @conference_service = ConferenceService.new @mock_repository, team_service
    end

    it 'should save the conference' do
      conference = Conference.build 1

      expect( @mock_repository ).to receive( :create ).with conference

      @conference_service.save conference
    end

    it 'should save the conference accolade' do
      conference          = Conference.build 1
      conference_accolade = ConferenceAccolade.build 1, 2, ConferenceAccolades::CottonBowlChampions

      conference.accolades = conference_accolade

      expect( @mock_repository ).to receive( :create ).with conference
      expect( @mock_repository ).to receive( :create ).with conference_accolade

      @conference_service.save conference
    end

    it 'should save many conference accolades' do
      conference           = Conference.build 1
      conference_accolade1 = ConferenceAccolade.build 1, 2, ConferenceAccolades::CottonBowlChampions
      conference_accolade2 = ConferenceAccolade.build 1, 2, ConferenceAccolades::LibertyBowlChampions

      conference.accolades = [conference_accolade1, conference_accolade2]

      expect( @mock_repository ).to receive( :create ).with conference
      expect( @mock_repository ).to receive( :create ).with conference_accolade1
      expect( @mock_repository ).to receive( :create ).with conference_accolade2

      @conference_service.save conference
    end

    it 'should save the conference stats' do
      conference       = Conference.build 1
      conference_stats = ConferenceStats.build 1, 2, Bowls::OrangeBowl

      conference.stats = conference_stats

      expect( @mock_repository ).to receive( :create ).with conference
      expect( @mock_repository ).to receive( :create ).with conference_stats

      @conference_service.save conference
    end

    it 'should save many conference stats' do
      conference        = Conference.build 1
      conference_stats1 = ConferenceStats.build 1, 2, Bowls::OrangeBowl
      conference_stats2 = ConferenceStats.build 1, 2, Bowls::NCFOChampionship

      conference.stats = [conference_stats1, conference_stats2]

      expect( @mock_repository ).to receive( :create ).with conference
      expect( @mock_repository ).to receive( :create ).with conference_stats1
      expect( @mock_repository ).to receive( :create ).with conference_stats2

      @conference_service.save conference
    end

    it 'should save the conference team' do
      conference      = Conference.build 1
      conference_team = ConferenceTeam.build 1, 2

      conference.teams = conference_team

      expect( @mock_repository ).to receive( :create ).with conference
      expect( @mock_repository ).to receive( :create ).with conference_team

      @conference_service.save conference
    end

    it 'should save the conference team and team ' do
      conference      = Conference.build 1
      conference_team = ConferenceTeam.build 1, 2
      team            = Team.build 2

      conference_team.team = team
      conference.teams     = conference_team

      expect( @mock_repository ).to receive( :create ).with conference
      expect( @mock_repository ).to receive( :create ).with conference_team
      expect( @mock_repository ).to receive( :create ).with team

      @conference_service.save conference
    end

    it 'should save many conference teams' do
      conference       = Conference.build 1
      conference_team1 = ConferenceTeam.build 1, 2
      conference_team2 = ConferenceTeam.build 1, 5

      conference.teams = [conference_team1, conference_team2]

      expect( @mock_repository ).to receive( :create ).with conference
      expect( @mock_repository ).to receive( :create ).with conference_team1
      expect( @mock_repository ).to receive( :create ).with conference_team2

      @conference_service.save conference
    end
  end
end
