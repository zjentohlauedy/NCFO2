location = File.dirname __FILE__
$: << "#{location}"

require 'utils'

describe Utils do
  describe '::roll' do
    it 'should return a randomly chosen array index given an array' do
      value = Utils::roll [ 1, 1, 1, 1, 1 ]

      expect( value ).to be_a Integer
      expect( value ).to be >= 0
      expect( value ).to be <= 4
    end

    it 'should return an index based on the weights of each index in the passed array' do
      value = Utils::roll [ 0, 0, 0, 0, 0, 123, 0, 0, 0, 0 ]

      expect( value ).to eq 5
    end

    it 'should consider every index of the passed array' do
      value = Utils::roll [ 5, 0, 0, 15, 0, 0, 0, 8, 11, 0, 30, 0, 6 ]

      expect( [ 0, 3, 7, 8, 10, 12 ] ).to include value
    end
  end

  describe '::get_db' do
    it 'should return a sqlite3 database instance' do
      db = Utils::get_db "ncfo_test.db"

      expect( db ).to_not be_nil
      expect( db ).to     be_a   SQLite3::Database

      expect( db.results_as_hash  ).to be true
      expect( db.type_translation ).to be true
    end
  end
end
