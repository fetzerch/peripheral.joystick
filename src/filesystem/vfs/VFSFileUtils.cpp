/*
 *      Copyright (C) 2015 Garrett Brown
 *      Copyright (C) 2015 Team XBMC
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

// This must be #defined before libXBMC_addon.h to fix compile
#if !defined(_WIN32)
  #include <sys/stat.h>
  #define __stat64 stat64
#endif

#include "VFSFileUtils.h"

#include "kodi/libXBMC_addon.h"

#include <assert.h>

using namespace JOYSTICK;

#ifndef S_ISDIR
  #define S_ISDIR(mode)  ((((mode)) & 0170000) == (0040000))
#endif

#ifndef S_ISLNK
  #define S_ISLNK(mode)  ((((mode)) & 0170000) == (0120000))
#endif

CVFSFileUtils::CVFSFileUtils(ADDON::CHelper_libXBMC_addon* frontend)
  : m_frontend(frontend)
{
  assert(m_frontend);
}

bool CVFSFileUtils::Exists(const std::string& url)
{
  return m_frontend->FileExists(url.c_str(), true);
}

bool CVFSFileUtils::Stat(const std::string& url, STAT_STRUCTURE& buffer)
{
  struct __stat64 frontendBuffer = { };
  if (m_frontend->StatFile(url.c_str(), &frontendBuffer))
  {
    buffer.deviceId         = frontendBuffer.st_dev;
    buffer.size             = frontendBuffer.st_size;
#if defined(__APPLE__)
    buffer.accessTime       = frontendBuffer.st_atimespec;
    buffer.modificationTime = frontendBuffer.st_mtimespec;
    buffer.statusTime       = frontendBuffer.st_ctimespec;
#elif defined(_WIN32)
    buffer.accessTime       = frontendBuffer.st_atime;
    buffer.modificationTime = frontendBuffer.st_mtime;
    buffer.statusTime       = frontendBuffer.st_ctime;
#else
    buffer.accessTime       = frontendBuffer.st_atim;
    buffer.modificationTime = frontendBuffer.st_mtim;
    buffer.statusTime       = frontendBuffer.st_ctim;
#endif
    buffer.isDirectory      = S_ISDIR(frontendBuffer.st_mode);
    buffer.isSymLink        = S_ISLNK(frontendBuffer.st_mode);
    buffer.isHidden         = false; // TODO
    return true;
  }
  return false;
}

bool CVFSFileUtils::Rename(const std::string& url, const std::string& newUrl)
{
  // TODO
  //return m_frontend->RenameFile(url.c_str(), newUrl.c_str());
  return false;
}

bool CVFSFileUtils::Delete(const std::string& url)
{
  return m_frontend->DeleteFile(url.c_str());
}
