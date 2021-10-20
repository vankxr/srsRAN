#
# Copyright 2013-2021 Software Radio Systems Limited
#
# This file is part of srsRAN
#
# srsRAN is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as
# published by the Free Software Foundation, either version 3 of
# the License, or (at your option) any later version.
#
# srsRAN is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Affero General Public License for more details.
#
# A copy of the GNU Affero General Public License can be found in
# the LICENSE file in the top-level directory of this distribution
# and at http://www.gnu.org/licenses/.
#

message(STATUS "FINDING MYSQL.")
if(NOT MYSQL_FOUND)
  pkg_check_modules (MYSQL_PKG mysqlclient)
  
  find_path( MYSQL_INCLUDE_DIRS
      NAMES mysql.h
      PATHS /usr/include/mysql
            /usr/local/include/mysql
            /usr/mysql/include/mysql )

  find_library( MYSQL_LIBRARIES
      NAMES mysqlclient
      PATHS /lib/mysql
            /lib64/mysql
            /usr/lib/mysql
            /usr/lib64/mysql
            /usr/local/lib/mysql
            /usr/local/lib64/mysql
            /usr/mysql/lib/mysql  
            /usr/mysql/lib64/mysql )

if(MYSQL_INCLUDE_DIRS AND MYSQL_LIBRARIES)
  set(MYSQL_FOUND TRUE CACHE INTERNAL "libmysqlclient found")
  message(STATUS "Found libmysqlclient: ${MYSQL_INCLUDE_DIRS}, ${MYSQL_LIBRARIES}")
else(MYSQL_INCLUDE_DIRS AND MYSQL_LIBRARIES)
  set(MYSQL_FOUND FALSE CACHE INTERNAL "libmysqlclient found")
  message(STATUS "libmysqlclient not found.")
endif(MYSQL_INCLUDE_DIRS AND MYSQL_LIBRARIES)

mark_as_advanced( MYSQL_INCLUDE_DIRS MYSQL_LIBRARIES )

endif(NOT MYSQL_FOUND)
