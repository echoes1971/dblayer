#pragma once

#include <iostream>
#include <map>
#include <string>
#include <typeinfo>
#include <vector>
using namespace std;

template<class T>
class Factory {
  public:
    Factory(bool verbose=false) { this->_verbose=verbose; }
    virtual ~Factory() {
        if(this->_verbose) cout << typeid(T).name() << "Factory::~" << typeid(T).name() << "Factory: start." << endl;
        // Cleaning the cache
        for(const auto& elem : this->_cache) {
            if(this->_verbose) printf("%sFactory::~%sFactory: deleting %s => %s\n",typeid(T).name(),typeid(T).name(),elem.first.c_str(), elem.second->getName().c_str() );
            delete elem.second;
        }
        this->_cache.clear();
        if(this->_verbose) cout << typeid(T).name() << "Factory::~" << typeid(T).name() << "Factory: end." << endl;
    }

    void setVerbose(bool verbose=true) { this->_verbose = verbose; }

    void registerClass(const string &tablename, T* clazz) {
        string tmpIndex=tablename;
        // IF exists => delete the old definition
        if(this->_cache.find(tmpIndex)!=this->_cache.end())
            delete this->_cache[tmpIndex];
        this->_cache[tmpIndex] = clazz;
    }
    vector<T*> getRegisteredTypes() const {
        vector<T*> ret;
        for(const auto& elem : this->_cache) {
            ret.push_back(elem.second);
        }
        return ret;
    }

    T* getClazz(const string& tablename, bool create_new_instance=true) const {
        string nomeTabella(tablename.c_str());
        if(this->_verbose) cout << typeid(T).name() << "Factory::getClazz: nomeTabella=" << nomeTabella << endl;
        if( this->_cache.find(nomeTabella) != this->_cache.end() ) {
            if(this->_verbose) cout << typeid(T).name() << "Factory::getClazz: found!" << endl;
            T* dbe = this->_cache.at(nomeTabella);
            return create_new_instance ? dbe->createNewInstance() : dbe;
        }
        return new T(tablename);
    }
    T* getClazzByTypeName(const string& typeName, bool caseSensitive=true, bool create_new_instance=true) const {
        if(this->_verbose) cout << typeid(T).name() << "Factory::getClazzByTypeName: typeName=" << typeName << endl;
        for(const auto& elem : this->_cache) {
            if(this->_verbose) cout << " " << elem.first << ": " << elem.second->name() << " type: " << typeid(elem.second).name() << endl;
            if(caseSensitive && elem.second->name()==typeName) {
                return create_new_instance ? elem.second->createNewInstance() : elem.second;
            }
        }
        return new T();
    }

    string toString(string prefix="\n") const {
        string ret;
        ret.append(prefix + "<"+typeid(T).name()+"Factory>");
        for(const auto& elem : this->_cache) {
            ret.append(prefix + " <clazz>");
            ret.append(prefix + "  <key>" + elem.first + "</key>" );
            ret.append(prefix + "  <value>" +  elem.second->name() + "</value>");
            ret.append(prefix + " </clazz>");
        }
        ret.append(prefix + "</"+typeid(T).name()+"Factory>");
        return ret;
    }

  protected:
    map<string,T*> _cache;
  private:
    bool _verbose;
};
