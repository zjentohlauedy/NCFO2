require 'sqlite3'

module Utils
  def self.roll weights
    prng = Random.new

    x = prng.rand weights.inject( :+ ) + 1

    weights.each_index do |idx|
      return idx unless (x -= weights[idx]) > 0
    end
  end

  def self.get_db file
    db = SQLite3::Database.new 'ncfo_test.db'

    db.results_as_hash  = true
    db.type_translation = true

    db
  end
end
