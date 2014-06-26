class Repository

  def initialize db
    @db = db
  end

  def start_transaction
    @db.transaction
  end

  def cancel_transaction
    @db.rollback
  end

  def end_transaction
    @db.commit
  end

  def create persistable
    raise 'object must be persistable' unless persistable.is_a? Persistable

    @db.execute persistable.build_insert, persistable.to_hash
  end

  def custom_create query, parameters = nil
    @db.execute query, parameters
  end

  def read persistable
    raise 'object must be persistable' unless persistable.is_a? Persistable

    data = @db.execute persistable.build_select, persistable.key_hash

    if data.nil? or data.length == 0; return nil end

    result = transform_hash data

    persistable.from_hash result[0]

    return persistable
  end

  def custom_read query, parameters = nil
    data = @db.execute query, parameters

    return transform_hash data
  end

  def update persistable
    raise 'object must be persistable' unless persistable.is_a? Persistable

    @db.execute persistable.build_update, persistable.to_hash
  end

  def custom_update query, parameters = nil
    @db.execute query, parameters
  end

  def delete persistable
    raise 'object must be persistable' unless persistable.is_a? Persistable

    @db.execute persistable.build_delete, persistable.key_hash
  end

  def custom_delete query, parameters = nil
    @db.execute query, parameters
  end

  private
  def transform_hash db_hash
    result = []

    db_hash.each do |element|
      hash = {}

      element.each do|key, value|
        hash.store key.downcase.to_sym, value
      end

      result.push hash
    end

    return result
  end
end
