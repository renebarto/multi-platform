#pragma once

#include <osal/exports.h>
#include <vector>
//#include <osal/FlagOperators.h>

#if defined(WIN_MSVC)
#include "osal/windows/Files.h"
#elif defined(WIN_MINGW)
#include "osal/mingw/Files.h"
#elif defined(DARWIN)
#include "osal/darwin/Files.h"
#elif defined(LINUX)
#include "osal/linux/Files.h"
#endif

namespace OSAL {
namespace Files {

//class OSAL_EXPORT TemporaryFile
//{
//public:
//    TemporaryFile();
//    ~TemporaryFile();
//    void Close();
//    std::iostream * GetStream();
//    std::string const & GetPath() const;
//
//private:
//    std::shared_ptr<std::iostream> _stream;
//    std::string _path;
//};
//
//class OSAL_EXPORT FileList
//{
//public:
//    FileList()
//    {}
//
//    const std::vector<std::string> & GetItems() { return _items; }
//    void Add(const std::string & item) { _items.push_back(item); }
//
//private:
//    std::vector<std::string> _items;
//};
//
//enum class SearchOption : int
//{
//    Normal = 0x00,
//    Recursive = 0x01,
//    IncludeHidden = 0x02,
//    IncludeSymbolicLinks = 0x04,
//};
//
//FileList OSAL_EXPORT ScanForFiles(const std::string & directory,
//                                  const std::string & searchPattern,
//                                  SearchOption searchOption = SearchOption::Normal);
//FileList OSAL_EXPORT ScanForDirectories(const std::string & directory,
//                                        const std::string & searchPattern,
//                                        SearchOption searchOption = SearchOption::Normal);
//bool OSAL_EXPORT GetFilesInCurrentDirectory(FileList & result,
//                                            const std::string & path,
//                                            const std::string & searchPattern,
//                                            SearchOption searchOption);

} // namespace Files
} // namespace OSAL

//template<>
//struct is_flag<OSAL::Files::SearchOption>
//{
//    static constexpr bool value = true;
//};
