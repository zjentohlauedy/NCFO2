#! /usr/bin/env ruby

location = File.dirname __FILE__
$: << "#{location}"

require 'utils'
require 'positions'
require 'player_offense_ratings'


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



if true

# max speed
# wr + cb
# rb + s
# te + lb
# qb
# ol + dl


prng = Random.new
# index       0    1     2      3      4      5      6      7      8      9     10     11    12   13   14   15
# value       6   13    19     25     31     38     44     50     56     63     69     75    81   88   94  100
# pct      .001  .01    .1     .2     .5      3      8     13     18     20     20     12     5   .2  .05  .01
Weights1 = [  1,  10,  100,   200,   500,  3000,  8000, 13000, 18000, 20000, 20000, 12000, 5000, 200,  50,  10 ] # wr + cb

# pct      .005  .05    .2     .5      3      7     12     18     24     18     10      6     1   .1  .01 .005
Weights2 = [  5,  50,  200,   500,  3000,  7000, 12000, 18000, 24000, 18000, 10000,  6000, 1000, 100,  10,   5 ] # rb + s

# pct       .02  .04    .9      6     10     18     20     20     15      7      3     .1   .04  .02  .01 .005
Weights3 = [ 20,  40,  900,  6000, 10000, 18000, 20000, 20000, 15000,  7000,  3000,   100,   40,  20,  10,   5 ] # te + lb

# pct       .01   .1     2     10     23     25     16     12      8      3      1    .05   .01 .004 .002 .001
Weights4 = [ 10, 100, 2000, 10000, 23000, 25000, 16000, 12000,  8000,  3000,  1000,    50,   10,   4,   2,   1 ] # qb

# pct        .1    1    15     27     25     15     10      5      2     .5     .1    .02  .008 .002 .001 .001
Weights5 = [100,1000,15000, 27000, 25000, 15000, 10000,  5000,  2000,   500,   100,    20,    8,   2,   1,   1 ] # ol + dl


arr1 = [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ]
arr2 = [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ]
arr3 = [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ]
arr4 = [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ]
arr5 = [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ]

(1..6).each do
  roll1 = Utils.roll Weights1
  roll2 = Utils.roll Weights2
  roll3 = Utils.roll Weights3
  roll4 = Utils.roll Weights4
  roll5 = Utils.roll Weights5

  arr1[roll1] += 1
  arr2[roll2] += 1
  arr3[roll3] += 1
  arr4[roll4] += 1
  arr5[roll5] += 1
end

puts "   0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15"

arr1.each do |x|
  printf "%4d ", x
end

puts ""

arr2.each do |x|
  printf "%4d ", x
end

puts ""

arr3.each do |x|
  printf "%4d ", x
end

puts ""

arr4.each do |x|
  printf "%4d ", x
end

puts ""

arr5.each do |x|
  printf "%4d ", x
end

puts ""

exit

end



if false

arr = []

(1..4).each do |x|
    rb = PlayerOffenseRatings.generate x, Positions::Runningback

    arr.push rb
end

(5..8).each do |x|
    wr = PlayerOffenseRatings.generate x, Positions::WideReceiver

    arr.push wr
end

(9..10).each do |x|
    te = PlayerOffenseRatings.generate x, Positions::TightEnd

    arr.push te
end

#arr.sort! { |x,y| x[:score] <=> y[:score] }


puts " ID  BC  RC  Score"

arr.each do |player|
  printf "%3d  %2d  %2d  %3d\n",
    player.player_id,
    player.ball_control,
    player.receiving,
    0
end

end
