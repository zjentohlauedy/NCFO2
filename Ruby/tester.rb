#! /usr/bin/env ruby

location = File.dirname __FILE__
$: << "#{location}"

require 'positions'
require 'player'
require 'player_ratings'
require 'player_quarterback_ratings'
require 'player_offense_ratings'
require 'player_defense_ratings'
require 'player_kicking_ratings'


players = []

# (1..3000).each do
#   player                     = Player.build 0
#   player.position            = Positions::Quarterback
#   player.ratings             = PlayerRatings.generate 0, Positions::Quarterback
#   player.quarterback_ratings = PlayerQuarterbackRatings.generate 0

#   players.push player
# end

# (1..10000).each do
#   player                 = Player.build 0
#   player.position        = Positions::Runningback
#   player.ratings         = PlayerRatings.generate 0, Positions::Runningback
#   player.offense_ratings = PlayerOffenseRatings.generate 0, Positions::Runningback

#   players.push player
# end

# (1..10000).each do
#   player                 = Player.build 0
#   player.position        = Positions::WideReceiver
#   player.ratings         = PlayerRatings.generate 0, Positions::WideReceiver
#   player.offense_ratings = PlayerOffenseRatings.generate 0, Positions::WideReceiver

#   players.push player
# end

(1..10000).each do
  player                 = Player.build 0
  player.position        = Positions::TightEnd
  player.ratings         = PlayerRatings.generate 0, Positions::TightEnd
  player.offense_ratings = PlayerOffenseRatings.generate 0, Positions::TightEnd

  players.push player
end

# (1..10000).each do
#   player                 = Player.build 0
#   player.position        = Positions::DefensiveLine
#   player.ratings         = PlayerRatings.generate 0, Positions::DefensiveLine
#   player.defense_ratings = PlayerDefenseRatings.generate 0, Positions::DefensiveLine

#   players.push player
# end

# (1..10000).each do
#   player                 = Player.build 0
#   player.position        = Positions::Linebacker
#   player.ratings         = PlayerRatings.generate 0, Positions::Linebacker
#   player.defense_ratings = PlayerDefenseRatings.generate 0, Positions::Linebacker

#   players.push player
# end

# (1..10000).each do
#   player                 = Player.build 0
#   player.position        = Positions::Cornerback
#   player.ratings         = PlayerRatings.generate 0, Positions::Cornerback
#   player.defense_ratings = PlayerDefenseRatings.generate 0, Positions::Cornerback

#   players.push player
# end

# (1..10000).each do
#   player                 = Player.build 0
#   player.position        = Positions::Safety
#   player.ratings         = PlayerRatings.generate 0, Positions::Safety
#   player.defense_ratings = PlayerDefenseRatings.generate 0, Positions::Safety

#   players.push player
# end


players.each do |player|

  acceleration = [player.ratings.run_speed, player.ratings.rush_power].max

  score  = player.ratings.max_speed * 3
  score += player.offense_ratings.receiving * 5
  score += acceleration * 2

  puts score
end


# 20 slots

# QB: PC x 8 + MS x 5 + PS x 3 + (RS<>RP) + HP + PA + APB
# RB: MS x 8 + HP x 5 + (RS<>RP) x 2 + BC x 2 + REC x 3
# WR: REC x 8 + MS x 6 + (RS<>RP) x 2 + BC x 2 + HP x 2
# TE: HP x 7 + REC x 7 + MS x 3 + BC x 2 + (RS<>RP)
# OL: HP x 13 + MS x 5 + (RS<>RP) x 2
# DL: HP x 11 + MS x 5 + (RS<>RP) x 2 + INT + QUI
# LB: HP x 8 + MS x 5 + (RS<>RP) x 2 + INT x 3 + QUI
# CB: MS x 8 + INT x 5 + (RS<>RP) x 3 + HP x 2 + QUI
#  S: MS x 7 + INT x 6 + (RS<>RP) x 2 + HP x 3 + QUI

#  K: KA x 15 + MS x 3 + AKB + HP
#  P: KA x 15 + MS x 3 + AKB + HP

# Ability Scores (10 slots)
#
# Passing:   PC x 5 + PS x 3 + PA + APB
# Rushing:   MS x 4 + HP x 3 + ACC x 2 + BC (QB gets default of 44)
# Receiving: REC x 5 + MS x 3 + ACC x 2
# Pass Rush: HP x 6 + MS x 2 + ACC + QUI
# Coverage:  INT x 5 + MS x 3 + ACC + QUI



#       Bad   Average   Good    Great
# QB: 0-100  100-135  135-150  150-300
# RB: 0-115  115-150  150-170  170-300
# WR: 0-130  130-180  180-190  190-300
# TE: 0-110  110-150  150-170  170-300
# OL: 0-110  110-175  175-190  190-300
# DL: 0-110  110-165  165-180  180-300
# LB: 0-110  100-150  150-170  170-300
# CB: 0-130  130-170  170-185  185-300
#  S: 0-130  130-170  170-180  180-300
#  K:
#  P:

#              Bad   Average   Good    Great
# Passing:    0-50    50-70   70-80   80-150
# Rushing:    0-50    50-80   80-90   90-150
# Receiving:  0-50    50-85   85-100 100-150
# Pass Rush:  0-50    50-80   80-90   90-150
# Coverage:   0-50    50-90   90-100 100-150
