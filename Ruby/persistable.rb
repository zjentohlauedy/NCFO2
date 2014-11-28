class Persistable

  def initialize table = nil, fields = [], keys = []
    @TableName  = table
    @FieldNames = fields
    @KeyFields  = keys
  end

  def build_insert
    "INSERT INTO #{@TableName} ( #{@FieldNames.join ', '} ) VALUES ( #{insert_placeholders} )"
  end

  def build_select
    "SELECT #{@FieldNames.join ', '} FROM #{@TableName} WHERE #{where_placeholders @KeyFields}"
  end

  def build_select_all
    "SELECT #{@FieldNames.join ', '} FROM #{@TableName}"
  end

  def custom_select table, select_fields, where_fields
    "SELECT #{select_fields.join ', '} FROM #{table} WHERE #{where_placeholders where_fields}"
  end

  def build_update
    "UPDATE #{@TableName} SET #{set_placeholders @FieldNames - @KeyFields} WHERE #{where_placeholders @KeyFields}"
  end

  def custom_update table, update_fields, where_fields
    "UPDATE #{table} SET #{set_placeholders update_fields} WHERE #{where_placeholders where_fields}"
  end

  def build_delete
    "DELETE FROM #{@TableName} WHERE #{where_placeholders @KeyFields}"
  end

  def custom_delete table, where_fields
    "DELETE FROM #{table} WHERE #{where_placeholders where_fields}"
  end

  def insert_placeholders
    @FieldNames.map{ |f| ':' + f.downcase }.join ', '
  end

  def where_placeholders fields
    fields.map{ |f| "#{f} = :" + f.downcase }.join ' AND '
  end

  def set_placeholders fields
    fields.map{ |f| "#{f} = :" + f.downcase }.join ', '
  end
end
