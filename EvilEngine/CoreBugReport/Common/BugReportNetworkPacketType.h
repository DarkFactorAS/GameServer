#ifndef EVILENGINE_COREBUGREPORT_COMMON_BUGREPORTNETWORKPACKETTYPE
#define EVILENGINE_COREBUGREPORT_COMMON_BUGREPORTNETWORKPACKETTYPE 1

namespace BugReportNetworkPacketType
{
  enum PacketTYpe
  {
    PacketType_BugReport,
    PacketType_BugReportId,
    PacketType_BugReportFile,
    PacketType_BugReportStatusFile,
    PacketType_BugReportPartialFile,
  };
}

#endif /// EVILENGINE_COREBUGREPORT_COMMON_BUGREPORTNETWORKPACKETTYPE