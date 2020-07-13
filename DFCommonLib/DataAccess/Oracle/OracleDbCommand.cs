using System;
using System.Data;
using Oracle.ManagedDataAccess.Client;

namespace BGCommonLib.DataAccess
{
    public interface IOracleBluDbCommand : IBluDbCommand
    {
        /// <summary>
        /// 
        /// </summary>
        /// <param name="name"></param>
        /// <param name="value"></param>
        /// <param name="dbType"></param>
        /// <param name="direction"></param>
        [Obsolete("Use AddParameter(string, object, DbType, ParameterDirection")]
        void AddOracleParameter(
            string name,
            object value,
            OracleDbType dbType,
            ParameterDirection direction);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="name"></param>
        /// <param name="value"></param>
        /// <param name="dbType"></param>
        [Obsolete("Use AddParameter(string, object, DbType")]
        void AddOracleParameter(
            string name,
            object value,
            OracleDbType dbType
            );

        /// <summary>
        /// Adds an output parameter to the collection of parameters for this DbProcedure.
        /// </summary>
        /// <param name="name"> Specifies the parameter name. </param>
        /// <param name="dbType"> Specifies the data type. </param>
        [Obsolete("Bruk AddParameter istedet")]
        void AddOracleOutputParameter(
            string name,
            OracleDbType dbType);

        /// <summary>
        /// Adds an output parameter to the collection of parameters for this DbProcedure.
        /// Use this for VARCHAR2 output variables.
        /// </summary>
        /// <param name="name"> Specifies the parameter name. </param>
        /// <param name="dbType"> Specifies the data type. </param>
        /// <param name="size"> Specifies the size of the parameter. </param>
        [Obsolete("Bruk AddParameter istedet")]
        void AddOracleOutputParameter(
            string name,
            OracleDbType dbType,
            int size);
    }

    /// <summary>
    /// Represents an SQL statement that is executed while connected to a data source.
    /// </summary>
    public class OracleDbCommand : IDbCommand, IOracleBluDbCommand
    {
        private readonly int _fetchRowCount;
        private readonly OracleCommand _command;
        private readonly OracleDbConnection _connection;
        private readonly bool _disposeConnection;
        private bool _disposed;

        //        /// <summary>
        //        /// Creates a new instance of DbCommand using default connection. 
        //        /// After the execution the connection is automaticly disposed.
        //        /// </summary>
        //        /// <param name="commandText"> Specifies the command text. </param>
        //        public DbCommand(
        //            string commandText)
        //            : this(commandText, (CommandType)CommandType.Text)
        //        {
        //            disposeConnection = true;
        //#if _ConnectionSpy
        //      Trace.Write(" DbCommand Constructor - dispose:" + disposeConnection.ToString() + Environment.NewLine);
        //#endif
        //        }

        ///// <summary>
        ///// Creates a new instance of DbCommand using default connection.
        ///// After the execution the connection is automaticly disposed.
        ///// </summary>
        ///// <param name="commandText"> Specifies the command text. </param>
        ///// <param name="commandType"> Specifies the command type. </param>
        //public DbCommand(
        //    string commandText,
        //    CommandType commandType)
        //    : this(commandText, commandType, new DbConnection())
        //{
        //    disposeConnection = true;
        //}

        /// <summary>
        /// Creates a new instance of DbCommand. The connection is not changed or disposed
        /// </summary>
        /// <param name="commandText"> Specifies the command text. </param>
        /// <param name="connection"> Specifies the data connection. </param>
        public OracleDbCommand(
            string commandText,
            OracleDbConnection connection, bool disposeConnectionAfterExecute)
            : this(commandText, CommandType.Text, connection)
        {
            _disposeConnection = disposeConnectionAfterExecute;
        }


        public OracleDbCommand(
          string commandText,
          OracleDbConnection connection, bool disposeConnectionAfterExecute, int fetchRowCount)
            : this(commandText, CommandType.Text, connection)
        {
            _disposeConnection = disposeConnectionAfterExecute;
            _fetchRowCount = fetchRowCount;
        }



        /// <summary>
        /// Creates a new instance of DbCommand. The connection is not changed or disposed
        /// </summary>
        /// <param name="commandText"> Specifies the command text. </param>
        /// <param name="connection"> Specifies the data connection. </param>
        /// <param name="readerFetchRowCount">Specifies the number of rows to get from DB in one fetch</param>
        //public DbCommand(
        //    string commandText,
        //    DbConnection connection,
        //    int readerFetchRowCount)
        //    : this(commandText, CommandType.Text, connection)
        //{
        //    _fetchRowCount = readerFetchRowCount;
        //    _disposeConnection = false;
        //}

        /// <summary>
        /// Creates a new instance of DbCommand using a given connection string.
        /// </summary>
        /// <param name="commandText"></param>
        /// <param name="connectionString"></param>
        /// <param name="readerFetchRowCount"></param>
        /// <param name="killConnection"></param>
        //public DbCommand(
        //    string commandText,
        //    string connectionString,
        //    int readerFetchRowCount,
        //    bool killConnection = true)
        //    : this(commandText, CommandType.Text, new DbConnection(connectionString))
        //{
        //    _fetchRowCount = readerFetchRowCount;
        //    _disposeConnection = killConnection;
        //}

        /// <summary>
        /// Creates a new instance of DbCommand.
        /// </summary>
        /// <param name="commandText"> Specifies the command text. </param>
        /// <param name="commandType"> Specifies the command type. </param>
        /// <param name="connection"> Specifies the data connection. </param>
        public OracleDbCommand(
            string commandText,
            CommandType commandType,
            OracleDbConnection connection)
        {
            _disposed = false;
            if (connection == null)
                throw new ArgumentNullException("connection");
            if (commandText == null)
                throw new ArgumentNullException("commandText");

            _connection = connection;

            _command = new OracleCommand(commandText, _connection.OracleConnection);
            _command.CommandType = commandType;
        }


        /// <summary>
        /// Exposes the BindByName property of the inner command object.
        /// </summary>
        public bool BindByName
        {
            get { return _command.BindByName; }
            set { _command.BindByName = value; }
        }

        /// <summary>
        /// Exposes the BindArrayCount property of the inner command object.
        /// </summary>
        /// 
        public int ArrayBindCount
        {
            get { return _command.ArrayBindCount; }
            set { _command.ArrayBindCount = value; }
        }

        ///// <summary>
        ///// Gets the inner command used by the DbCommand.
        ///// </summary>
        //protected OracleCommand Command
        //{
        //    get { return _command; }
        //}

        #region IDbCommand Members

        /// <summary>
        /// Gets or sets the connection associated with the DbCommand.
        /// </summary>
        IDbConnection IDbCommand.Connection
        {
            get { return _connection.OracleConnection; }
            set { throw new NotImplementedException("Can only be set on creationtime"); }
        }


        /// <summary>
        /// Executes the query, and returns a IDataReader.
        /// </summary>
        /// <param name="behavior"></param>
        /// <returns>IDataReader</returns>
        public virtual IDataReader ExecuteReader(CommandBehavior behavior)
        {
            if (_command.Connection.State != ConnectionState.Open)
                throw new InvalidOperationException("Cannot call ExecuteReader while connection is closed.");

            OracleDataReader reader = _command.ExecuteReader(behavior);
            if (_fetchRowCount > 0)
            {
                reader.FetchSize = _command.RowSize * _fetchRowCount;
            }

            return reader;
        }

        /// <summary>
        /// Gets or sets the name of the commandText.
        /// </summary>
        public string CommandText
        {
            get { return _command.CommandText; }
            set
            {
                if (value == null)
                    throw new ArgumentNullException("value");

                _command.CommandText = value;
            }
        }

        /// <summary>
        /// Gets or sets the name of the command timeout.
        /// </summary>
        int IDbCommand.CommandTimeout
        {
            get { return _command.CommandTimeout; }
            set { _command.CommandTimeout = value; }
        }

        /// <summary>
        /// Gets or sets the name of the command type.
        /// </summary>
        public CommandType CommandType
        {
            get { return _command.CommandType; }
            set { _command.CommandType = value; }
        }

        /// <summary>
        /// Gets the command parameters.
        /// </summary>
        public IDataParameterCollection Parameters
        {
            get { return _command.Parameters; }
        }


        /// <summary>
        /// Creates a new command parameter.
        /// </summary>
        /// <returns> A new command parameter. </returns>
        IDbDataParameter IDbCommand.CreateParameter()
        {
            return _command.CreateParameter();
        }

        /// <summary>
        /// Executes the stored procedure against the connection and returns the number of rows affected.
        /// </summary>
        /// <returns> The number of rows affected. </returns>
        public virtual int ExecuteNonQuery()
        {
            return _command.ExecuteNonQuery();
        }

        /// <summary>
        /// Executes the query, and returns the first column of the first row in the result set returned by the query. Extra columns or rows are ignored.
        /// </summary>
        /// <returns>
        /// The first column of the first row in the result set, or a null reference if the result set is empty.
        /// </returns>
        public virtual object ExecuteScalar()
        {
            return _command.ExecuteScalar();
        }


        IDataReader IDbCommand.ExecuteReader()
        {
            return ((IDbCommand)this).ExecuteReader(CommandBehavior.Default);
        }

        void IDbCommand.Cancel()
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Prepares the DbCommand.
        /// </summary>
        public void Prepare()
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Gets or sets the transaction associated with the DbCommand.
        /// </summary>
        IDbTransaction IDbCommand.Transaction
        {
            get { throw new NotImplementedException(); }
            set { throw new NotImplementedException(); }
        }

        UpdateRowSource IDbCommand.UpdatedRowSource
        {
            get { throw new NotImplementedException(); }
            set { throw new NotImplementedException(); }
        }

        /// <summary>
        /// Disposes the resources used by this instance of DbCommand.
        /// </summary>
        public void Dispose()
        {
            if (!_disposed)
            {
                _command.Dispose();
                if (_disposeConnection)
                {
                    _connection.Dispose();
                }

                _disposed = true;
            }
        }

        #endregion

        /// <summary>
        /// Adds a parameter to the collection of parameters for this DbProcedure.
        /// </summary>
        /// <param name="name"> Specifies the parameter name. </param>
        /// <param name="dbType"> Specifies the data type. </param>
        /// <returns>The parameter</returns>
        public IDbDataParameter AddParameter(string name, DbType dbType)
        {
            IDbDataParameter p = ((IDbCommand)this).CreateParameter();
            p.ParameterName = name;
            p.DbType = dbType;

            AddParameter(p);
            return p;
        }


        /// <summary>
        /// Adds a parameter to the collection of parameters for this DbProcedure.
        /// </summary>
        /// <param name="parameter"> Specifies the parameter to add. </param>
        /// <returns>The parameter</returns>
        public IDbDataParameter AddParameter(
            IDbDataParameter parameter)
        {
            _command.Parameters.Add(parameter);
            return parameter;
        }

        /// <summary>
        /// Adds a parameter to the collection of parameters for this DbProcedure.
        /// </summary>
        /// <param name="name"> Specifies the parameter name. </param>
        /// <param name="value"> Specifies the parameter value. </param>
        /// <returns>The parameter</returns>
        public IDbDataParameter AddParameter(
            string name,
            object value)
        {
            string type = value == null ? "String" : value.GetType().Name;
            switch (type)
            {
                case "Int64":
                    return AddParameter(name, value, DbType.Int64, ParameterDirection.Input);
                case "Int32":
                case "Int16":
                    return AddParameter(name, value, DbType.Int32, ParameterDirection.Input);

                case "Decimal":
                    return AddParameter(name, value, DbType.Decimal, ParameterDirection.Input);

                case "Double":
                    return AddParameter(name, value, DbType.Double, ParameterDirection.Input);

                case "DateTime":
                    return AddParameter(name, value, DbType.Date, ParameterDirection.Input);

                case "Single":
                    return AddParameter(name, value, DbType.Single, ParameterDirection.Input);

                default:
                    return AddParameter(name, value, DbType.String, ParameterDirection.Input);
            }
        }

        /// <summary>
        /// Adds a parameter to the collection of parameters for this DbProcedure.
        /// </summary>
        /// <param name="name"> Specifies the parameter name. </param>
        /// <param name="value"> Specifies the parameter value. </param>
        /// <param name="dbType"> Specifies the data type. </param>
        /// <param name="direction"> Specifies the direction. </param>
        /// <returns>The parameter</returns>
        public IDbDataParameter AddParameter(
            string name,
            object value,
            DbType dbType,
            ParameterDirection direction)
        {
            IDbDataParameter p = AddParameter(name, dbType);
            p.Value = value;
            p.Direction = direction;


            return p;
        }

        //to save a long string in a clob OracleDbType.Clob is nessesary
        //not passing type means no ref to oracle in calling code is nessecary
        /// <summary>
        /// Adds a parameter to the collection of parameters for this DbProcedure.
        /// </summary>
        /// <param name="name"> Specifies the parameter name. </param>
        /// <param name="value"> Specifies the parameter value. </param>
        /// <returns>The parameter</returns>
        public void AddClobParameter(
            string name,
            object value)
        {
            var p = new OracleParameter();
            p.OracleDbType = OracleDbType.Clob;
            p.Value = value;
            p.ParameterName = name;
            p.Direction = ParameterDirection.Input;
            _command.Parameters.Add(p);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="name"></param>
        /// <param name="value"></param>
        /// <param name="dbType"></param>
        /// <param name="direction"></param>
        [Obsolete("Use AddParameter(string, object, DbType, ParameterDirection")]
        public void AddOracleParameter(
            string name,
            object value,
            OracleDbType dbType,
            ParameterDirection direction)
        {
            var p = new OracleParameter();
            p.OracleDbType = dbType;

            AddParameter(name, value, p.DbType, direction);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="name"></param>
        /// <param name="value"></param>
        /// <param name="dbType"></param>
        [Obsolete("Use AddParameter(string, object, DbType")]
        public void AddOracleParameter(
            string name,
            object value,
            OracleDbType dbType
            )
        {
            AddOracleParameter(name, value, dbType, ParameterDirection.Input);
        }


        /// <summary>
        /// Adds an output parameter to the collection of parameters for this DbProcedure.
        /// </summary>
        /// <param name="name"> Specifies the parameter name. </param>
        /// <param name="dbType"> Specifies the data type. </param>
        [Obsolete("Bruk AddParameter istedet")]
        public void AddOracleOutputParameter(
            string name,
            OracleDbType dbType)
        {
            int size;
            switch (dbType.ToString())
            {
                case "Int64":
                    size = sizeof(long);
                    break;
                case "Int32":
                case "Int16":
                    size = sizeof(int);
                    break;

                case "Decimal":
                    size = sizeof(decimal);
                    break;

                case "Date":
                case "Double":
                    size = sizeof(double);
                    break;

                default:
                    size = 2000;
                    break;
            }

            AddOracleOutputParameter(name, dbType, size);
        }

        /// <summary>
        /// Adds an output parameter to the collection of parameters for this DbProcedure.
        /// Use this for VARCHAR2 output variables.
        /// </summary>
        /// <param name="name"> Specifies the parameter name. </param>
        /// <param name="dbType"> Specifies the data type. </param>
        /// <param name="size"> Specifies the size of the parameter. </param>
        [Obsolete("Bruk AddParameter istedet")]
        public void AddOracleOutputParameter(
            string name,
            OracleDbType dbType,
            int size)
        {
            var p = new OracleParameter();
            p.ParameterName = name;
            p.Direction = ParameterDirection.Output;
            p.OracleDbType = dbType;
            p.Size = size;

            _command.Parameters.Add(p);
        }


        /// <summary>
        /// Executes the query, and returns the first column of the first row in the result set returned by the query. Extra columns or rows are ignored.
        /// </summary>
        /// <typeparam name="TReturnType"> Type to return. </typeparam>
        /// <returns>
        /// The first column of the first row in the result set, or a null reference if the result set is empty.
        /// </returns>
        public virtual TReturnType ExecuteScalar<TReturnType>()
        {

            object result = _command.ExecuteScalar();
            if (result != DBNull.Value && result != null)
                return (TReturnType)result;

            return default(TReturnType);
        }

        /// <summary>
        /// Executes the query, and returns a IDataReader.
        /// </summary>
        /// <returns>IDataReader</returns>
        public virtual IDataReader ExecuteReader()
        {
            return new OracleDbDataReader(((IDbCommand)this).ExecuteReader());
        }


        /// <summary>
        /// When set to -1, the clob-columns are included with the resultset, reducing the need to fetch each single row as a separate request to db. 
        /// Improves speed on data retrival, most gain is when a high latency connection is used (VPN)
        /// </summary>
        /// <param name="fetchSize"></param>
        public void SetInitialLOBFetchSize(int fetchSize)
        {
            _command.InitialLOBFetchSize = fetchSize;
        }
    }

    public class TimedDbCommand : OracleDbCommand
    {
        public TimedDbCommand(string commandText, OracleDbConnection connection, bool disposeConnectionAfterExecute) : base(commandText, connection, disposeConnectionAfterExecute)
        {
        }

        public TimedDbCommand(string commandText, OracleDbConnection connection, bool disposeConnectionAfterExecute, int fetchRowCount) : base(commandText, connection, disposeConnectionAfterExecute, fetchRowCount)
        {
        }

        public TimedDbCommand(string commandText, CommandType commandType, OracleDbConnection connection) : base(commandText, commandType, connection)
        {
        }


        public override TReturnType ExecuteScalar<TReturnType>()
        {
            //using (new ActivityTracingContext("ExecuteNonQuery<>: ", CommandText))
            {
                return base.ExecuteScalar<TReturnType>();
            }
        }

        public override int ExecuteNonQuery()
        {
            //using (new ActivityTracingContext("ExecuteNonQuery: ", CommandText))
            {
                return base.ExecuteNonQuery();
            }
        }

        public override object ExecuteScalar()
        {
            //using (new ActivityTracingContext("ExecuteScalar: ", CommandText))
            {
                return base.ExecuteScalar();
            }
        }

        public override IDataReader ExecuteReader()
        {
            //using (new ActivityTracingContext("ExecuteReader: ", CommandText))
            {
                return new TimedDbDataReader(((IDbCommand)this).ExecuteReader(), CommandText);
            }
        }

    }
}
