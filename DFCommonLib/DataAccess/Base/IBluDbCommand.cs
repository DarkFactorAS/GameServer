using System;
using System.Data;

namespace DFCommonLib.DataAccess
{
    public interface IBluDbCommand : IDisposable
    {
        /// <summary>
        /// Exposes the BindByName property of the inner command object.
        /// </summary>
        bool BindByName { get; set; }

        /// <summary>
        /// Exposes the BindArrayCount property of the inner command object.
        /// </summary>
        /// 
        int ArrayBindCount { get; set; }

        /// <summary>
        /// Gets or sets the name of the commandText.
        /// </summary>
        string CommandText { get; set; }

        /// <summary>
        /// Gets or sets the name of the command type.
        /// </summary>
        CommandType CommandType { get; set; }

        /// <summary>
        /// Gets the command parameters.
        /// </summary>
        IDataParameterCollection Parameters { get; }

        /// <summary>
        /// Executes the stored procedure against the connection and returns the number of rows affected.
        /// </summary>
        /// <returns> The number of rows affected. </returns>
        int ExecuteNonQuery();

        /// <summary>
        /// Executes the query, and returns the first column of the first row in the result set returned by the query. Extra columns or rows are ignored.
        /// </summary>
        /// <returns>
        /// The first column of the first row in the result set, or a null reference if the result set is empty.
        /// </returns>
        object ExecuteScalar();

        /// <summary>
        /// Prepares the DbCommand.
        /// </summary>
        void Prepare();


        /// <summary>
        /// Adds a parameter to the collection of parameters for this DbProcedure.
        /// </summary>
        /// <param name="name"> Specifies the parameter name. </param>
        /// <param name="dbType"> Specifies the data type. </param>
        /// <returns>The parameter</returns>
        IDbDataParameter AddParameter(string name, DbType dbType);

        /// <summary>
        /// Adds a parameter to the collection of parameters for this DbProcedure.
        /// </summary>
        /// <param name="parameter"> Specifies the parameter to add. </param>
        /// <returns>The parameter</returns>
        IDbDataParameter AddParameter(
            IDbDataParameter parameter);

        /// <summary>
        /// Adds a parameter to the collection of parameters for this DbProcedure.
        /// </summary>
        /// <param name="name"> Specifies the parameter name. </param>
        /// <param name="value"> Specifies the parameter value. </param>
        /// <returns>The parameter</returns>
        IDbDataParameter AddParameter(
            string name,
            object value);

        /// <summary>
        /// Adds a parameter to the collection of parameters for this DbProcedure.
        /// </summary>
        /// <param name="name"> Specifies the parameter name. </param>
        /// <param name="value"> Specifies the parameter value. </param>
        /// <param name="dbType"> Specifies the data type. </param>
        /// <param name="direction"> Specifies the direction. </param>
        /// <returns>The parameter</returns>
        IDbDataParameter AddParameter(
            string name,
            object value,
            DbType dbType,
            ParameterDirection direction);

        /// <summary>
        /// Adds a parameter to the collection of parameters for this DbProcedure.
        /// </summary>
        /// <param name="name"> Specifies the parameter name. </param>
        /// <param name="value"> Specifies the parameter value. </param>
        /// <returns>The parameter</returns>
        void AddClobParameter(
            string name,
            object value);

        /// <summary>
        /// Executes the query, and returns the first column of the first row in the result set returned by the query. Extra columns or rows are ignored.
        /// </summary>
        /// <typeparam name="TReturnType"> Type to return. </typeparam>
        /// <returns>
        /// The first column of the first row in the result set, or a null reference if the result set is empty.
        /// </returns>
        TReturnType ExecuteScalar<TReturnType>();

        /// <summary>
        /// Executes the query, and returns a IDataReader.
        /// </summary>
        /// <returns>IDataReader</returns>
        IDataReader ExecuteReader();

    }
}