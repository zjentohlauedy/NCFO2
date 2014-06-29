location = File.dirname __FILE__
$: << "#{location}"

require 'player_generator'
require 'player'
require 'team_player'
require 'positions'

describe PlayerGenerator do
  describe '#choose_number' do
    it 'should return a player number' do
      generator = PlayerGenerator.new

      number = generator.choose_number Positions::Quarterback, []

      expect( number ).to_not be_nil
      expect( number ).to     be_a   Integer
    end

    it 'should return a quarterback number if position is quarterback' do
      generator = PlayerGenerator.new

      number = generator.choose_number Positions::Quarterback, []

      expect( [*1..19] ).to include number
    end

    it 'should return a runningback number if position is runningback' do
      generator = PlayerGenerator.new

      number = generator.choose_number Positions::Runningback, []

      expect( [*30..39] ).to include number
    end

    it 'should return a wide receiver number if position is wide receiver' do
      generator = PlayerGenerator.new

      number = generator.choose_number Positions::WideReceiver, []

      expect( [*80..89] ).to include number
    end

    it 'should return a tight end number if position is tight end' do
      generator = PlayerGenerator.new

      number = generator.choose_number Positions::TightEnd, []

      expect( [*80..89] ).to include number
    end

    it 'should return a offensive linesman number if position is an offensive linesman' do
      generator = PlayerGenerator.new

      number = generator.choose_number Positions::OffensiveLine, []

      expect( [*60..79] ).to include number
    end

    it 'should return a defensive linesman number if position is an defensive linesman' do
      generator = PlayerGenerator.new

      number = generator.choose_number Positions::DefensiveLine, []

      expect( [*90..99] ).to include number
    end

    it 'should return a linebacker number if position is a linebacker' do
      generator = PlayerGenerator.new

      number = generator.choose_number Positions::Linebacker, []

      expect( [*50..59] ).to include number
    end

    it 'should return a cornerback number if position is a cornerback' do
      generator = PlayerGenerator.new

      number = generator.choose_number Positions::Cornerback, []

      expect( [*20..29] ).to include number
    end

    it 'should return a safety number if position is a safety' do
      generator = PlayerGenerator.new

      number = generator.choose_number Positions::Safety, []

      expect( [*40..49] ).to include number
    end

    it 'should return a kicker number if position is a kicker' do
      generator = PlayerGenerator.new

      number = generator.choose_number Positions::Kicker, []

      expect( [*1..19] ).to include number
    end

    it 'should return a punter number if position is a punter' do
      generator = PlayerGenerator.new

      number = generator.choose_number Positions::Punter, []

      expect( [*1..19] ).to include number
    end

    it 'should return a number not used by any other player' do
      generator = PlayerGenerator.new

      players = []

      (30..38).each do |num|
        tp = TeamPlayer.new
        p  = Player.new
        p.number = num
        tp.player = p
        players.push tp
      end

      number = generator.choose_number Positions::Runningback, players

      expect( number ).to eq 39
    end
  end

  describe '#generate_player' do
    it 'should return a player object with the given values' do
      generator = PlayerGenerator.new

      player = generator.generate_player 1, Positions::Quarterback, 12, 1

      expect( player ).to_not be_nil
      expect( player ).to     be_a   Player

      expect( player.player_id       ).to eq 1
      expect( player.position        ).to eq Positions::Quarterback
      expect( player.number          ).to eq 12
      expect( player.freshman_season ).to eq 1
    end

    it 'should return a player with a name' do
      generator = PlayerGenerator.new

      player = generator.generate_player 1, Positions::Quarterback, 12, 1

      expect( player.first_name ).to_not be_nil
      expect( player.first_name ).to     be_a   String

      expect( player.last_name ).to_not be_nil
      expect( player.last_name ).to     be_a   String
    end

    it 'should return a player with a face' do
      generator = PlayerGenerator.new

      player = generator.generate_player 1, Positions::Quarterback, 12, 1

      expect( player.face ).to_not be_nil
      expect( player.face ).to     be_a   Integer

      expect( [*0..81, *128..211] ).to include player.face
    end

    it 'should return a player with maturity' do
      generator = PlayerGenerator.new

      player = generator.generate_player 1, Positions::Quarterback, 12, 1

      expect( player.maturity ).to_not be_nil
      expect( player.maturity ).to     be_a   Integer

      expect( [1, 2, 3, 4] ).to include player.maturity
    end

    it 'should return a player with ratings' do
      generator = PlayerGenerator.new

      player = generator.generate_player 1, Positions::Quarterback, 12, 1

      expect( player.ratings ).to_not be_nil
      expect( player.ratings ).to     be_a   PlayerRatings

      expect( player.ratings.player_id ).to eq( player.player_id ).and eq 1

      expect( player.ratings.run_speed  ).to_not be_nil
      expect( player.ratings.rush_power ).to_not be_nil
      expect( player.ratings.max_speed  ).to_not be_nil
      expect( player.ratings.hit_power  ).to_not be_nil
    end

    it 'should return a player with quarterback ratings if that player is a quarterback' do
      generator = PlayerGenerator.new

      player = generator.generate_player 1, Positions::Quarterback, 12, 1

      expect( player.quarterback_ratings ).to_not be_nil
      expect( player.quarterback_ratings ).to     be_a   PlayerQuarterbackRatings

      expect( player.quarterback_ratings.player_id ).to eq( player.player_id ).and eq 1

      expect( player.quarterback_ratings.pass_speed       ).to_not be_nil
      expect( player.quarterback_ratings.pass_control     ).to_not be_nil
      expect( player.quarterback_ratings.pass_accuracy    ).to_not be_nil
      expect( player.quarterback_ratings.avoid_pass_block ).to_not be_nil
    end

    it 'should return a player without offense, defense and kicking ratings if that player is a quarterback' do
      generator = PlayerGenerator.new

      player = generator.generate_player 1, Positions::Quarterback, 12, 1

      expect( player.offense_ratings ).to be_nil
      expect( player.defense_ratings ).to be_nil
      expect( player.kicking_ratings ).to be_nil
    end

    it 'should return a player with offense ratings if that player is a runningback' do
      generator = PlayerGenerator.new

      player = generator.generate_player 1, Positions::Runningback, 12, 1

      expect( player.offense_ratings ).to_not be_nil
      expect( player.offense_ratings ).to     be_a   PlayerOffenseRatings

      expect( player.offense_ratings.player_id ).to eq( player.player_id ).and eq 1

      expect( player.offense_ratings.ball_control ).to_not be_nil
      expect( player.offense_ratings.receiving    ).to_not be_nil
    end

    it 'should return a player without quarterback, defense or kicking ratings if that player is a runningback' do
      generator = PlayerGenerator.new

      player = generator.generate_player 1, Positions::Runningback, 12, 1

      expect( player.quarterback_ratings ).to be_nil
      expect( player.defense_ratings     ).to be_nil
      expect( player.kicking_ratings     ).to be_nil
    end

    it 'should return a player with offense ratings if that player is a wide receiver' do
      generator = PlayerGenerator.new

      player = generator.generate_player 1, Positions::WideReceiver, 12, 1

      expect( player.offense_ratings ).to_not be_nil
      expect( player.offense_ratings ).to     be_a   PlayerOffenseRatings

      expect( player.offense_ratings.player_id ).to eq( player.player_id ).and eq 1

      expect( player.offense_ratings.ball_control ).to_not be_nil
      expect( player.offense_ratings.receiving    ).to_not be_nil
    end

    it 'should return a player without quarterback, defense or kicking ratings if that player is a wide receiver' do
      generator = PlayerGenerator.new

      player = generator.generate_player 1, Positions::WideReceiver, 12, 1

      expect( player.quarterback_ratings ).to be_nil
      expect( player.defense_ratings     ).to be_nil
      expect( player.kicking_ratings     ).to be_nil
    end

    it 'should return a player with offense ratings if that player is a tight end' do
      generator = PlayerGenerator.new

      player = generator.generate_player 1, Positions::TightEnd, 12, 1

      expect( player.offense_ratings ).to_not be_nil
      expect( player.offense_ratings ).to     be_a   PlayerOffenseRatings

      expect( player.offense_ratings.player_id ).to eq( player.player_id ).and eq 1

      expect( player.offense_ratings.ball_control ).to_not be_nil
      expect( player.offense_ratings.receiving    ).to_not be_nil
    end

    it 'should return a player without quarterback, defense or kicking ratings if that player is a tight end' do
      generator = PlayerGenerator.new

      player = generator.generate_player 1, Positions::TightEnd, 12, 1

      expect( player.quarterback_ratings ).to be_nil
      expect( player.defense_ratings     ).to be_nil
      expect( player.kicking_ratings     ).to be_nil
    end

    it 'should return a player with defense ratings if that player is a defensive lineman' do
      generator = PlayerGenerator.new

      player = generator.generate_player 1, Positions::DefensiveLine, 12, 1

      expect( player.defense_ratings ).to_not be_nil
      expect( player.defense_ratings ).to     be_a   PlayerDefenseRatings

      expect( player.defense_ratings.player_id ).to eq( player.player_id ).and eq 1

      expect( player.defense_ratings.interceptions ).to_not be_nil
      expect( player.defense_ratings.quickness     ).to_not be_nil
    end

    it 'should return a player without quarterback, offense or kicking ratings if that player is a defensive lineman' do
      generator = PlayerGenerator.new

      player = generator.generate_player 1, Positions::DefensiveLine, 12, 1

      expect( player.quarterback_ratings ).to be_nil
      expect( player.offense_ratings     ).to be_nil
      expect( player.kicking_ratings     ).to be_nil
    end

    it 'should return a player with defense ratings if that player is a linebacker' do
      generator = PlayerGenerator.new

      player = generator.generate_player 1, Positions::Linebacker, 12, 1

      expect( player.defense_ratings ).to_not be_nil
      expect( player.defense_ratings ).to     be_a   PlayerDefenseRatings

      expect( player.defense_ratings.player_id ).to eq( player.player_id ).and eq 1

      expect( player.defense_ratings.interceptions ).to_not be_nil
      expect( player.defense_ratings.quickness     ).to_not be_nil
    end

    it 'should return a player without quarterback, offense or kicking ratings if that player is a linebacker' do
      generator = PlayerGenerator.new

      player = generator.generate_player 1, Positions::Linebacker, 12, 1

      expect( player.quarterback_ratings ).to be_nil
      expect( player.offense_ratings     ).to be_nil
      expect( player.kicking_ratings     ).to be_nil
    end

    it 'should return a player with defense ratings if that player is a cornerback' do
      generator = PlayerGenerator.new

      player = generator.generate_player 1, Positions::Cornerback, 12, 1

      expect( player.defense_ratings ).to_not be_nil
      expect( player.defense_ratings ).to     be_a   PlayerDefenseRatings

      expect( player.defense_ratings.player_id ).to eq( player.player_id ).and eq 1

      expect( player.defense_ratings.interceptions ).to_not be_nil
      expect( player.defense_ratings.quickness     ).to_not be_nil
    end

    it 'should return a player without quarterback, offense or kicking ratings if that player is a cornerback' do
      generator = PlayerGenerator.new

      player = generator.generate_player 1, Positions::Cornerback, 12, 1

      expect( player.quarterback_ratings ).to be_nil
      expect( player.offense_ratings     ).to be_nil
      expect( player.kicking_ratings     ).to be_nil
    end

    it 'should return a player with defense ratings if that player is a safety' do
      generator = PlayerGenerator.new

      player = generator.generate_player 1, Positions::Safety, 12, 1

      expect( player.defense_ratings ).to_not be_nil
      expect( player.defense_ratings ).to     be_a   PlayerDefenseRatings

      expect( player.defense_ratings.player_id ).to eq( player.player_id ).and eq 1

      expect( player.defense_ratings.interceptions ).to_not be_nil
      expect( player.defense_ratings.quickness     ).to_not be_nil
    end

    it 'should return a player without quarterback, offense or kicking ratings if that player is a safety' do
      generator = PlayerGenerator.new

      player = generator.generate_player 1, Positions::Safety, 12, 1

      expect( player.quarterback_ratings ).to be_nil
      expect( player.offense_ratings     ).to be_nil
      expect( player.kicking_ratings     ).to be_nil
    end

    it 'should return a player with kicking ratings if that player is a kicker' do
      generator = PlayerGenerator.new

      player = generator.generate_player 1, Positions::Kicker, 12, 1

      expect( player.kicking_ratings ).to_not be_nil
      expect( player.kicking_ratings ).to     be_a   PlayerKickingRatings

      expect( player.kicking_ratings.player_id ).to eq( player.player_id ).and eq 1

      expect( player.kicking_ratings.kicking_ability  ).to_not be_nil
      expect( player.kicking_ratings.avoid_kick_block ).to_not be_nil
    end

    it 'should return a player without quarterback, offense or defense ratings if that player is a kicker' do
      generator = PlayerGenerator.new

      player = generator.generate_player 1, Positions::Kicker, 12, 1

      expect( player.quarterback_ratings ).to be_nil
      expect( player.offense_ratings     ).to be_nil
      expect( player.defense_ratings     ).to be_nil
    end

    it 'should return a player with kicking ratings if that player is a punter' do
      generator = PlayerGenerator.new

      player = generator.generate_player 1, Positions::Punter, 12, 1

      expect( player.kicking_ratings ).to_not be_nil
      expect( player.kicking_ratings ).to     be_a   PlayerKickingRatings

      expect( player.kicking_ratings.player_id ).to eq( player.player_id ).and eq 1

      expect( player.kicking_ratings.kicking_ability  ).to_not be_nil
      expect( player.kicking_ratings.avoid_kick_block ).to_not be_nil
    end

    it 'should return a player without quarterback, offense or defense ratings if that player is a punter' do
      generator = PlayerGenerator.new

      player = generator.generate_player 1, Positions::Punter, 12, 1

      expect( player.quarterback_ratings ).to be_nil
      expect( player.offense_ratings     ).to be_nil
      expect( player.defense_ratings     ).to be_nil
    end

    it 'should return a player without quarterback, offense, defense or kicking ratings if that player is an offensice linesman' do
      generator = PlayerGenerator.new

      player = generator.generate_player 1, Positions::OffensiveLine, 12, 1

      expect( player.quarterback_ratings ).to be_nil
      expect( player.offense_ratings     ).to be_nil
      expect( player.defense_ratings     ).to be_nil
      expect( player.kicking_ratings     ).to be_nil
    end
  end
end
