location = File.dirname __FILE__
$: << "#{location}"

require 'persistable'

describe Persistable do
  describe '#build_insert' do
    it 'should return a well formed SQL insert statement' do
      persistable = Persistable.new 'TableName', %w(KeyField FieldName), %w(KeyField)
      expect( persistable.build_insert ).to eq 'INSERT INTO TableName ( KeyField, FieldName ) VALUES ( ?, ? )'
    end

    it 'should return a well formed SQL insert statement with multiple fields and key fields' do
      persistable = Persistable.new 'TableName', %w(KeyField1 KeyField2 FieldName1 FieldName2 FieldName3), %w(KeyField1 KeyField2)
      expect( persistable.build_insert ).to eq 'INSERT INTO TableName ( KeyField1, KeyField2, FieldName1, FieldName2, FieldName3 ) VALUES ( ?, ?, ?, ?, ? )'
    end
  end

  describe '#build_select' do
    it 'should return a well formed SQL select statement' do
      persistable = Persistable.new 'TableName', %w(KeyField FieldName), %w(KeyField)
      expect( persistable.build_select ).to eq 'SELECT KeyField, FieldName FROM TableName WHERE KeyField = ?'
    end

    it 'should return a well formed SQL select statement with multiple fields and key fields' do
      persistable = Persistable.new 'TableName', %w(KeyField1 KeyField2 FieldName1 FieldName2 FieldName3), %w(KeyField1 KeyField2)
      expect( persistable.build_select ).to eq 'SELECT KeyField1, KeyField2, FieldName1, FieldName2, FieldName3 FROM TableName WHERE KeyField1 = ? AND KeyField2 = ?'
    end
  end

  describe '#build_update' do
    it 'should return a well formed SQL update statement' do
      persistable = Persistable.new 'TableName', %w(KeyField FieldName), %w(KeyField)
      expect( persistable.build_update ).to eq 'UPDATE TableName SET FieldName = ? WHERE KeyField = ?'
    end

    it 'should return a well formed SQL update statement with multiple fields and key fields' do
      persistable = Persistable.new 'TableName', %w(KeyField1 KeyField2 FieldName1 FieldName2 FieldName3), %w(KeyField1 KeyField2)
      expect( persistable.build_update ).to eq 'UPDATE TableName SET FieldName1 = ?, FieldName2 = ?, FieldName3 = ? WHERE KeyField1 = ? AND KeyField2 = ?'
    end
  end

  describe '#build_delete' do
    it 'should return a well formed SQL update statement' do
      persistable = Persistable.new 'TableName', %w(KeyField FieldName), %w(KeyField)
      expect( persistable.build_delete ).to eq 'DELETE FROM TableName WHERE KeyField = ?'
    end

    it 'should return a well formed SQL update statement with multiple fields and key fields' do
      persistable = Persistable.new 'TableName', %w(KeyField1 KeyField2 FieldName1 FieldName2 FieldName3), %w(KeyField1 KeyField2)
      expect( persistable.build_delete ).to eq 'DELETE FROM TableName WHERE KeyField1 = ? AND KeyField2 = ?'
    end
  end

  describe '#custom_select' do
    it 'should return a well formed SQL select statement from the given criteria' do
      persistable = Persistable.new
      result = persistable.custom_select 'TableName', %w(SelectField1 SelectField1), %w(WhereField1 WhereField2)
      expect( result ).to eq 'SELECT SelectField1, SelectField1 FROM TableName WHERE WhereField1 = ? AND WhereField2 = ?'
    end
  end

  describe '#custom_update' do
    it 'should return a well formed SQL update statement from the given criteria' do
      persistable = Persistable.new
      result = persistable.custom_update 'TableName', %w(UpdateField1 UpdateField2), %w(WhereField1 WhereField2)
      expect( result ).to eq 'UPDATE TableName SET UpdateField1 = ?, UpdateField2 = ? WHERE WhereField1 = ? AND WhereField2 = ?'
    end
  end

  describe '#custom_delete' do
    it 'should return a well formed SQL update statement from the given criteria' do
      persistable = Persistable.new
      result = persistable.custom_delete 'TableName', %w(WhereField1 WhereField2)
      expect( result ).to eq 'DELETE FROM TableName WHERE WhereField1 = ? AND WhereField2 = ?'
    end
  end
end