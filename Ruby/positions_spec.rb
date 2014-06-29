location = File.dirname __FILE__
$: << "#{location}"

require 'positions'

describe Positions do
  describe '::string_value' do
    it 'should convert the given position to the string equivalent' do
      expect( Positions::string_value Positions::None          ).to eq 'N/A'
      expect( Positions::string_value Positions::Quarterback   ).to eq 'QB'
      expect( Positions::string_value Positions::Runningback   ).to eq 'RB'
      expect( Positions::string_value Positions::WideReceiver  ).to eq 'WR'
      expect( Positions::string_value Positions::TightEnd      ).to eq 'TE'
      expect( Positions::string_value Positions::OffensiveLine ).to eq 'OL'
      expect( Positions::string_value Positions::DefensiveLine ).to eq 'DL'
      expect( Positions::string_value Positions::Linebacker    ).to eq 'LB'
      expect( Positions::string_value Positions::Cornerback    ).to eq 'CB'
      expect( Positions::string_value Positions::Safety        ).to eq 'S'
      expect( Positions::string_value Positions::Kicker        ).to eq 'K'
      expect( Positions::string_value Positions::Punter        ).to eq 'P'
    end
  end
end
