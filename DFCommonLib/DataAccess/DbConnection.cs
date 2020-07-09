using System;
using System.Data;
using Oracle.ManagedDataAccess.Client;

namespace BGCommonLib.DataAccess
{
    /// <summary>
    /// Represents a Db-connection
    /// </summary>
    public sealed class DbConnection :
        IDbConnection,
        IDisposable
    {
        ///// <summary>
        ///// Creates a new instance of DbConnection using default connection.
        ///// </summary>
        ///// <remarks>Default sets the Connection.State to Open</remarks>
        //public DbConnection()
        //    : this(ConnectionState.Open, Db.GetPagaWebConnectionString())
        //{
        //}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="connectionString"></param>
        public DbConnection(string connectionString)
            : this(ConnectionState.Open, connectionString)
        {
        }

        ///// <summary>
        ///// 
        ///// </summary>
        //public DbConnection(ConnectionState state)
        //    : this(state, Db.GetPagaWebConnectionString())
        //{
        //}


        /// <summary>
        /// Creates a new instance of DbConnection
        /// </summary>
        /// <param name="state"></param>
        /// <param name="connectionString"></param>
        public DbConnection(ConnectionState state, string connectionString)
        //: base()
        {
            conn = new OracleConnection(connectionString);
            switch (state)
            {
                case ConnectionState.Open:
                    conn.Open();
                    break;

                case ConnectionState.Closed:
                    conn.Close();
                    break;

                default:
                    throw new NotImplementedException();
            }
        }

        private readonly OracleConnection conn = null;

        /// <summary>
        /// This is needed for the DbCommand
        /// </summary>
        internal OracleConnection OracleConnection
        {
            get { return conn; }
        }

        /// <summary>
        /// Gets the underlying connection as a IDbConnection
        /// </summary>
        public IDbConnection WrappedConnection
        {
            get { return conn; }
        }

        #region IDbConnection Members

        IDbTransaction IDbConnection.BeginTransaction(IsolationLevel il)
        {
            return conn.BeginTransaction(il);
        }

        /// <summary>
        /// Begins a transaction using the default IsolationLevel, ReadCommitted
        /// </summary>
        /// <returns></returns>
        IDbTransaction IDbConnection.BeginTransaction()
        {
            return ((IDbConnection)this).BeginTransaction(IsolationLevel.ReadCommitted);
        }

        void IDbConnection.ChangeDatabase(string databaseName)
        {
            throw new Exception("The method or operation is not implemented.");
        }

        void IDbConnection.Close()
        {
            conn.Close();
        }

        string IDbConnection.ConnectionString
        {
            get { return conn.ConnectionString; }
            set { conn.ConnectionString = value; }
        }

        int IDbConnection.ConnectionTimeout
        {
            get { return conn.ConnectionTimeout; }
        }

        IDbCommand IDbConnection.CreateCommand()
        {
            return conn.CreateCommand();
        }

        string IDbConnection.Database
        {
            get { throw new Exception("The method or operation is not implemented."); }
        }

        void IDbConnection.Open()
        {
            conn.Open();
        }

        ConnectionState IDbConnection.State
        {
            get { return conn.State; }
        }

        #endregion

        #region IDisposable Members

        /// <summary>
        /// Disposes the resources used by this instance of DbConnection.
        /// </summary>
        public void Dispose()
        {
            conn.Dispose();
        }

        #endregion
    }
}
