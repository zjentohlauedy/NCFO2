location = File.dirname __FILE__
$: << "#{location}"

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

describe 'TeamService' do
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
