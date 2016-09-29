#!/usr/bin/env ruby
#
location = File.dirname __FILE__

$: << "#{location}"
require 'json'


def get_bowl(filename)
  case filename
  when /Cotton_Bowl/;       return 1
  when /Orange_Bowl/;       return 2
  when /Rose_Bowl/;         return 3
  when /Sugar_Bowl/;        return 4
  when /Fiesta_Bowl/;       return 5
  when /Liberty_Bowl/;      return 6
  when /NCFO_Championship/; return 7
  when /All_American_Bowl/; return 8
  end

  return 0
end


teams = []

ARGV.each do |file|
  road_team = {}
  home_team = {}

  road_team[:bowl] = get_bowl file
  home_team[:bowl] = road_team[:bowl]

  File::foreach file do |line|
    if line.match /^ +U[A-Z][A-Z] +U[A-Z][A-Z]/
      (road_team[:name], home_team[:name]) = line.strip.split(/ +/)
    end

    if line.match /^ +[A-Z][A-Z][A-Z]\. +[A-Z][A-Z][A-Z]\./
      (road_team[:name], home_team[:name]) = line.strip.gsub('.', '').split(/ +/)
    end

    if line.match /^Total Offense:/
      (ignore, ignore, home_team[:yards_allowed], road_team[:yards_allowed]) = line.strip.split(/ +/)
    end
  end

  teams.push road_team
  teams.push home_team
end

puts JSON::generate teams

