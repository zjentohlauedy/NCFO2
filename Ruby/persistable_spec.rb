location = File.dirname __FILE__
$: << "#{location}"

require 'persistable'

describe Persistable do
  describe '#build_insert' do
    it 'should return a well formed SQL insert statement' do
      persistable = Persistable.new 'TableName', %w(KeyField FieldName), %w(KeyField)
      expect( persistable.build_insert ).to eq 'INSERT INTO TableName ( KeyField, FieldName ) VALUES ( :keyfield, :fieldname )'
    end

    it 'should return a well formed SQL insert statement with multiple fields and key fields' do
      persistable = Persistable.new 'TableName', %w(KeyField1 KeyField2 FieldName1 FieldName2 FieldName3), %w(KeyField1 KeyField2)
      expect( persistable.build_insert ).to eq 'INSERT INTO TableName ( KeyField1, KeyField2, FieldName1, FieldName2, FieldName3 ) VALUES ( :keyfield1, :keyfield2, :fieldname1, :fieldname2, :fieldname3 )'
    end
  end

  describe '#build_select' do
    it 'should return a well formed SQL select statement' do
      persistable = Persistable.new 'TableName', %w(KeyField FieldName), %w(KeyField)
      expect( persistable.build_select ).to eq 'SELECT KeyField, FieldName FROM TableName WHERE KeyField = :keyfield'
    end

    it 'should return a well formed SQL select statement with multiple fields and key fields' do
      persistable = Persistable.new 'TableName', %w(KeyField1 KeyField2 FieldName1 FieldName2 FieldName3), %w(KeyField1 KeyField2)
      expect( persistable.build_select ).to eq 'SELECT KeyField1, KeyField2, FieldName1, FieldName2, FieldName3 FROM TableName WHERE KeyField1 = :keyfield1 AND KeyField2 = :keyfield2'
    end
  end

  describe '#build_update' do
    it 'should return a well formed SQL update statement' do
      persistable = Persistable.new 'TableName', %w(KeyField FieldName), %w(KeyField)
      expect( persistable.build_update ).to eq 'UPDATE TableName SET FieldName = :fieldname WHERE KeyField = :keyfield'
    end

    it 'should return a well formed SQL update statement with multiple fields and key fields' do
      persistable = Persistable.new 'TableName', %w(KeyField1 KeyField2 FieldName1 FieldName2 FieldName3), %w(KeyField1 KeyField2)
      expect( persistable.build_update ).to eq 'UPDATE TableName SET FieldName1 = :fieldname1, FieldName2 = :fieldname2, FieldName3 = :fieldname3 WHERE KeyField1 = :keyfield1 AND KeyField2 = :keyfield2'
    end
  end

  describe '#build_delete' do
    it 'should return a well formed SQL delete statement' do
      persistable = Persistable.new 'TableName', %w(KeyField FieldName), %w(KeyField)
      expect( persistable.build_delete ).to eq 'DELETE FROM TableName WHERE KeyField = :keyfield'
    end

    it 'should return a well formed SQL delete statement with multiple fields and key fields' do
      persistable = Persistable.new 'TableName', %w(KeyField1 KeyField2 FieldName1 FieldName2 FieldName3), %w(KeyField1 KeyField2)
      expect( persistable.build_delete ).to eq 'DELETE FROM TableName WHERE KeyField1 = :keyfield1 AND KeyField2 = :keyfield2'
    end
  end

  describe '#custom_select' do
    it 'should return a well formed SQL select statement from the given criteria' do
      persistable = Persistable.new
      result = persistable.custom_select 'TableName', %w(SelectField1 SelectField1), %w(WhereField1 WhereField2)
      expect( result ).to eq 'SELECT SelectField1, SelectField1 FROM TableName WHERE WhereField1 = :wherefield1 AND WhereField2 = :wherefield2'
    end
  end

  describe '#custom_update' do
    it 'should return a well formed SQL update statement from the given criteria' do
      persistable = Persistable.new
      result = persistable.custom_update 'TableName', %w(UpdateField1 UpdateField2), %w(WhereField1 WhereField2)
      expect( result ).to eq 'UPDATE TableName SET UpdateField1 = :updatefield1, UpdateField2 = :updatefield2 WHERE WhereField1 = :wherefield1 AND WhereField2 = :wherefield2'
    end
  end

  describe '#custom_delete' do
    it 'should return a well formed SQL delete statement from the given criteria' do
      persistable = Persistable.new
      result = persistable.custom_delete 'TableName', %w(WhereField1 WhereField2)
      expect( result ).to eq 'DELETE FROM TableName WHERE WhereField1 = :wherefield1 AND WhereField2 = :wherefield2'
    end
  end

  describe '#build_select_all' do
    it 'should return a well formed SQL select statement without a where clause' do
      persistable = Persistable.new 'TableName', %w(KeyField1 KeyField2 FieldName1 FieldName2 FieldName3), %w(KeyField1 KeyField2)
      expect( persistable.build_select_all ).to eq 'SELECT KeyField1, KeyField2, FieldName1, FieldName2, FieldName3 FROM TableName'
    end
  end
end
