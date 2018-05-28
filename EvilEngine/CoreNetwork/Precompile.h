#include "EvilEngine/CoreLib/BasicTypes/Definitions/PlatformDefinition.h"

#include "EvilEngine/CoreLib/BasicTypes/String/CoreString.h"
#include "EvilEngine/CoreLib/BasicTypes/BinaryStream/BinaryStream.h"
#include "EvilEngine/CoreIO/Logger/Logger.h"
#include "EvilEngine/CoreLib/Assert/DFAssert.h"

#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/smart_ptr/detail/atomic_count.hpp>
#include <vector>
#include <algorithm>
