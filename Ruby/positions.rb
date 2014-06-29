module Positions
  None          =  0
  Quarterback   =  1
  Runningback   =  2
  WideReceiver  =  3
  TightEnd      =  4
  OffensiveLine =  5
  DefensiveLine =  6
  Linebacker    =  7
  Cornerback    =  8
  Safety        =  9
  Kicker        = 10
  Punter        = 11

  def self.string_value position
    case position
    when None;          return 'N/A'
    when Quarterback;   return 'QB'
    when Runningback;   return 'RB'
    when WideReceiver;  return 'WR'
    when TightEnd;      return 'TE'
    when OffensiveLine; return 'OL'
    when DefensiveLine; return 'DL'
    when Linebacker;    return 'LB'
    when Cornerback;    return 'CB'
    when Safety;        return 'S'
    when Kicker;        return 'K'
    when Punter;        return 'P'
    end
  end
end
