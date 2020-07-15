
using System;
using System.Collections;
using System.Data;
using System.Diagnostics;
using MySql.Data.MySqlClient;

namespace DFCommonLib.DataAccess
{
    public class MySQLDbDataReader : BaseDbDataReader
    {
        private readonly IDataReader _reader;
        private readonly string _commandText;
        private readonly Stopwatch _stopwatch;

        public MySQLDbDataReader(IDataReader reader, string commandText) : base(reader)
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
                /*
                var mySqlReader = _reader as MySqlDataReader;
                if (mySqlReader != null)
                {
                    return mySqlReader.RowCount;
                }
                */
                return 0;
            }
            set
            {
                /*
                var mySqlReader = _reader as MySqlDataReader;
                if (mySqlReader != null)
                {
                    mySqlReader.RowCount = value;
                }
                */
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