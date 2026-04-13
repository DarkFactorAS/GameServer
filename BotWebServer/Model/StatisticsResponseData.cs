namespace BotWebServer.Model
{
    public class StatisticsResponseData
    {
        public static uint Ok = 0;
        public static uint ErrorNotLoggedIn = 1;
        public static uint UnknownError = 2;

        public uint errorId { get; set; }
        public string message { get; set; }

        public StatisticsResponseData()
        {
        }

        public StatisticsResponseData(uint errorId, string message)
        {
            this.errorId = errorId;
            this.message = message;
        }
    }
}
