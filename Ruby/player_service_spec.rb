location = File.dirname __FILE__
$: << "#{location}"

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

describe 'PlayerService' do
  before :each do
    @mock_repository = double( 'Repository' )
    @player_service  = PlayerService.new @mock_repository
  end

  describe '#save' do
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
