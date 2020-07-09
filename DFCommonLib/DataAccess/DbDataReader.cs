using System;
using System.Collections;
using System.Data;
using System.Diagnostics;
using Oracle.ManagedDataAccess.Client;

namespace BGCommonLib.DataAccess
{
    /// <summary>
    /// Represents an SQL function that is executed while connected to a data source.
    /// This class is not inheritable.
    /// </summary>
    public class DbDataReader :
        IDataReader,
        IDisposable,
        IEnumerable
    {
        private readonly IDataReader reader;

        /// <summary>
        /// Create reader using specified command.
        /// </summary>
        /// <param name="reader"></param>
        public DbDataReader(IDataReader reader)
        {
            if (reader == null)
                throw new ArgumentNullException("reader");

            this.reader = reader;
        }

        #region IDataReader Members

        public virtual void Dispose()
        {
            reader.Dispose();
        }

        void IDataReader.Close()
        {
            reader.Close();
        }

        int IDataReader.Depth
        {
            get { return reader.Depth; }
        }

        DataTable IDataReader.GetSchemaTable()
        {
            return reader.GetSchemaTable();
        }

        bool IDataReader.IsClosed
        {
            get { return reader.IsClosed; }
        }

        bool IDataReader.NextResult()
        {
            return reader.NextResult();
        }

        /// <summary>
        /// Advanced the reader to the next record.
        /// </summary>
        /// <returns>true or false</returns>
        public bool Read()
        {
            return reader.Read();
        }

        int IDataReader.RecordsAffected
        {
            get { return reader.RecordsAffected; }
        }

        int IDataRecord.FieldCount
        {
            get { return reader.FieldCount; }
        }

        bool IDataRecord.GetBoolean(int i)
        {
            return reader.GetBoolean(i);
        }

        byte IDataRecord.GetByte(int i)
        {
            return reader.GetByte(i);
        }

        long IDataRecord.GetBytes(int i, long fieldOffset, byte[] buffer, int bufferoffset, int length)
        {
            return reader.GetBytes(i, fieldOffset, buffer, bufferoffset, length);
        }

        char IDataRecord.GetChar(int i)
        {
            return reader.GetChar(i);
        }

        long IDataRecord.GetChars(int i, long fieldoffset, char[] buffer, int bufferoffset, int length)
        {
            return reader.GetChars(i, fieldoffset, buffer, bufferoffset, length);
        }

        IDataReader IDataRecord.GetData(int i)
        {
            return reader.GetData(i);
        }

        string IDataRecord.GetDataTypeName(int i)
        {
            return reader.GetDataTypeName(i);
        }

        DateTime IDataRecord.GetDateTime(int i)
        {
            return reader.GetDateTime(i);
        }

        decimal IDataRecord.GetDecimal(int i)
        {
            return reader.GetDecimal(i);
        }

        double IDataRecord.GetDouble(int i)
        {
            return reader.GetDouble(i);
        }

        Type IDataRecord.GetFieldType(int i)
        {
            return reader.GetFieldType(i);
        }

        float IDataRecord.GetFloat(int i)
        {
            return reader.GetFloat(i);
        }

        Guid IDataRecord.GetGuid(int i)
        {
            return reader.GetGuid(i);
        }

        short IDataRecord.GetInt16(int i)
        {
            return reader.GetInt16(i);
        }

        int IDataRecord.GetInt32(int i)
        {
            return reader.GetInt32(i);
        }

        long IDataRecord.GetInt64(int i)
        {
            return reader.GetInt64(i);
        }

        string IDataRecord.GetName(int i)
        {
            return reader.GetName(i);
        }

        int IDataRecord.GetOrdinal(string name)
        {
            return reader.GetOrdinal(name);
        }

        /// <summary>
        /// Get the string value of the specified field from the reader.
        /// </summary>
        /// <param name="i">Specified field</param>
        /// <returns>String value from the reader</returns>
        public string GetString(int i)
        {
            return reader.GetString(i);
        }

        object IDataRecord.GetValue(int i)
        {
            return reader.GetValue(i);
        }

        int IDataRecord.GetValues(object[] values)
        {
            return reader.GetValues(values);
        }

        bool IDataRecord.IsDBNull(int i)
        {
            return reader.IsDBNull(i);
        }

        /// <summary>
        /// Gets the column with the specified name.
        /// </summary>
        /// <param name="name">Column name</param>
        /// <returns>Column as object</returns>
        public object this[string name]
        {
            get { return reader[name]; }
        }

        /// <summary>
        /// Gets the column located at the specified index
        /// </summary>
        /// <param name="i">The index of the column to get</param>
        /// <returns>Column as object</returns>
        public object this[int i]
        {
            get { return reader[i]; }
        }

        #endregion

        #region IEnumerable Members

        ///<summary>
        ///Returns an enumerator that iterates through a collection.
        ///</summary>
        ///
        ///<returns>
        ///An <see cref="T:System.Collections.IEnumerator"></see> object that can be used to iterate through the collection.
        ///</returns>
        ///<filterpriority>2</filterpriority>
        public IEnumerator GetEnumerator()
        {
            return ((IEnumerable)reader).GetEnumerator();
        }

        #endregion

        /// <summary>
        /// Get value at given column, as specified type
        /// If DBNull, return "default" value for type
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="name"></param>
        /// <returns></returns>
        public T GetValue<T>(string name)
        {
            return (T)this[name];
        }
    }


    public class TimedDbDataReader : DbDataReader
    {
        private readonly IDataReader _reader;
        private readonly string _commandText;
        private readonly Stopwatch _stopwatch;

        public TimedDbDataReader(IDataReader reader, string commandText) : base(reader)
        {
            _reader = reader;
            _commandText = commandText;

            //ActivityTracing.AddActivityTrace("DbDataReader", ActivityTracing.FilterMessage(commandText));
            _stopwatch = Stopwatch.StartNew();
        }

        public long FetchSize
        {
            get
            {
                var oracleReader = _reader as OracleDataReader;
                if (oracleReader != null)
                {
                    return oracleReader.FetchSize;
                }
                return 0;

            }
            set
            {
                var oracleReader = _reader as OracleDataReader;
                if (oracleReader != null)
                {
                    oracleReader.FetchSize = value;
                }
            }
        }


        public override void Dispose()
        {
            _stopwatch.Stop();
            //ActivityTracing.AddActivityTrace("DbDataReader", "Done reading: " + ActivityTracing.FilterMessage(_commandText), _stopwatch.ElapsedMilliseconds);
            base.Dispose();
        }
    }
}