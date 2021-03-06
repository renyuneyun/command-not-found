/*
    This file is part of command-not-found.
    Copyright (C) 2011 Matthias Maennich <matthias@maennich.net>

    command-not-found is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    command-not-found is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with command-not-found.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TDB_H_
#define TDB_H_

#include <fcntl.h>
#include <tdb.h>
#include <string>
#include <vector>

#include "db.h"
#include "custom_exceptions.h"

namespace cnf {
class TdbDatabase: public Database {
public:
    explicit TdbDatabase(const std::string& id,
                         const bool readonly,
                         const std::string& basepath);
    virtual void storePackage(const Package& p);
    virtual void getPackages(const std::string& search, std::vector<Package>& result ) const;
    virtual void truncate();
    virtual ~TdbDatabase();
    static void getCatalogs(const std::string& database_path, std::vector<std::string>& result);
private:
    TDB_CONTEXT* m_tdbFile;
    const std::string m_databaseName;
};

class TdbKeyValue {
public:
    explicit TdbKeyValue(const std::string& key, const std::string& value);
    explicit TdbKeyValue();
    void setKey(const std::string& key);
    void setKey(const TDB_DATA& key);
    void setValue(const std::string& key);
    void setValue(const TDB_DATA& key);
    ~TdbKeyValue();
    const TDB_DATA& key() const {
        return m_key;
    }
    const TDB_DATA& value() const {
        return m_value;
    }
    std::string key_str() const {
        if (m_key.dptr) {
            return std::string(reinterpret_cast<const char*>(m_key.dptr));
        } else {
            return std::string();
        }
    }
    std::string value_str() const {
        if (m_value.dptr) {
            return std::string(reinterpret_cast<const char*>(m_value.dptr));
        } else {
            return std::string();
        }
    }
private:
    TDB_DATA m_key;
    TDB_DATA m_value;
    TdbKeyValue(const TdbKeyValue&);
    TdbKeyValue& operator=(const TdbKeyValue&);
};

inline unsigned char* getWritableUCString(const std::string& aString);

}

#endif /* TDB_H_ */
