#ifndef _inc_filetools
#define _inc_filetools

bool FileExists(std::string file_name) {
    FILE *fp;
    fp = fopen(file_name.c_str(),"r");
    if (fp==NULL) {
            return false;
    }
    fclose(fp);
    return true;
}

bool FileExists(std::ostringstream& file_name) {
    return FileExists(file_name.str());
}

#include <errno.h>
#include <sys/file.h>
#include <fcntl.h>
#include <unistd.h>

int lock(std::string fname) {
    

    struct flock fl;
    fl.l_type = F_WRLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0;
    fl.l_pid = getpid();
    
    int fd;
    if (FileExists(fname)==true) {
        fd = open(fname.c_str(),O_RDONLY);
        if (fd>=0) {
                fcntl(fd,F_SETLKW,&fl);
        }
        return fd;
    } else {
        return -1;
    }
}

void unlock(int fd) {
    struct flock fl;
    fl.l_type = F_UNLCK;
    fl.l_pid = getpid();
    fcntl(fd,F_SETLK,&fl);
    close(fd);
}

template <typename T> void savedoublevector(vector<vector<T> >& D,std::string fname) {
    int i,j;
    int fd;

    do {
            fd = lock(fname);
            cout << "lock_fd=" << fd << endl;
            usleep(100000);
    } while (fd<0);

    ofstream vstream;

    vstream.open(fname.c_str(),ios_base::trunc); 

    vstream << D.size() << endl;

    for (i=0;i<(int)D.size();i++) {
        vstream << D[i].size() << "\t";
        for (j=0;j<(int)D[i].size();j++) {
            vstream << D[i][j] << "\t";
        }
        vstream << endl;
    }
    vstream.close();
    unlock(fd);
}
    
#define opt_ow 0x01
#define opt_app 0x02
    
template <typename T> void loaddoublevector(vector<vector<T> >& D,std::string fname,int opt) {

    if (FileExists(fname)==false) {
        D.resize(0);
        return;
    }
    int i,j,k;
    int listsize;
    int fd = lock(fname);
    ifstream vstream;
    vstream.open(fname.c_str(),ios_base::in);
    if (vstream.fail()==true) return; 

    vstream >> listsize;
    if (opt==opt_app) {
        k = D.size();
    } else {
        k=0;
    }
    D.resize(listsize+k);

    for (i=0;i<listsize;i++) {
        vstream >> j;
        D[i+k].resize(j);
        for (j=0;j<(int)D[i+k].size();j++) {
            vstream >> D[i+k][j];
            //cout << D[i+k][j] << endl;
        }
        //cout << "len of D[" << i << "]=" << D[i+k].size() << endl;
    }
    vstream.close();
    unlock(fd);
    //distinctvectorset(D);
    //distinct(D);
}

template <typename T> void loaddoublevector(vector<vector<T> >& D,std::string fname) {
    loaddoublevector(D,fname,opt_ow);
}

template <typename T> void savearray(T* vol,int m,std::string fname) {
    
    int fd = lock(fname);
    ofstream vstream;
    vstream.open(fname.c_str(),ios_base::trunc);
    int i;

    vstream << m << endl;
    //vstream.precision(std::numeric_limits<longlongfloat>::digits10);

    for (i=1;i<=m;i++) {
        vstream << vol[i] << endl;
    }
    vstream.close();
    unlock(fd);
}

template <typename T> void loadarray(T* vol,int max,std::string fname) {

    int fd;
    fd = lock(fname);
    //cout << fname << endl;
    //cout << "lockfd=" << fd << endl;
    
    ifstream vstream;
    vstream.open(fname.c_str(),ios_base::in);
    if (!vstream) {
        //error?
        cout << "vstream error?" << endl;
        cout << fname << endl;
        unlock(fd);
        fd = lock(fname);
        vstream.open(fname.c_str(),ios_base::in);
    }
    
    //vstream.precision(std::numeric_limits<longlongfloat>::digits10);
    int i;
    int m=0;
    vstream >> m;
    for (i=1;i<=min(m,max);i++) {
        vstream >> vol[i];
    }
    vstream.close();
    unlock(fd);
}


void SaveLattice(mat_ZZ& L,std::string fname) {
    ofstream logstream;
    logstream.open(fname.c_str(),ios_base::trunc);
    logstream << L;
	logstream.close();
}

void SaveLattice(mat_ZZ& L,std::ostringstream& fname) {
    SaveLattice(L,fname.str());
}

void LoadLattice(mat_ZZ& L,std::string fname) {

    ifstream logstream;
    logstream.open(fname.c_str(),ios_base::in);
    try {
        logstream >> L;
    }
    catch (std::exception e) {
        cout << e.what() << endl;
        L.SetDims(1,1);
    }
    logstream.close();
}

void LoadLattice(mat_ZZ& L,std::ostringstream& fname) {
    LoadLattice(L,fname.str());
}

template <typename T> void SaveElement(T& L,std::string fname) {
    ofstream logstream;
    logstream.open(fname.c_str(),ios_base::trunc);
    logstream << L;
	logstream.close();
}

template <typename T> void SaveElement(T& L,std::ostringstream& fname) {
    SaveElement(L,fname.str());
}

template <typename T> void LoadElement(T& L,std::string fname) {
    ifstream logstream;
    logstream.open(fname.c_str(),ios_base::in);
    logstream >> L;
	logstream.close();
}

template <typename T> void LoadElement(T& L,std::ostringstream& fname) {
    LoadElement(L,fname.str());
}

#include <boost/algorithm/string.hpp>

std::string ReadConf(const std::string filename, const std::string arg1) {
  ifstream is(filename, ios_base::in);
  std::string line;
  while(getline(is, line)){
    if(line.length() > 1){
      if(line[0] != '#'){
        std::vector<std::string> list;
        boost::split(list, line, boost::is_any_of("="));
        if(list[0] == arg1) return list[1];
      }
    }
  }
  return "";
}

void WriteConf(const std::string filename, const std::string arg1, const std::string arg2){
  std::vector<std::string> lines;
  std::ifstream is(filename, std::ios_base::in);
  std::string line;
  while(getline(is, line)) lines.push_back(line);
  is.close();

  std::ofstream os(filename, std::ios_base::trunc);
  char exist_flag = 0;
  for(auto itr = lines.begin(); itr != lines.end(); ++itr){
    if(itr->length() > 1){
      if((*itr)[0] != '#'){
        std::vector<std::string> list;
        boost::split(list, line, boost::is_any_of("="));
        if(list[0] == arg1){
          *itr = arg1 + "=" + arg2;
          exist_flag = 1;
        }
      }
    }
    os << *itr << std::endl;
  }
  if(!exist_flag) os << arg1 << "=" << arg2 << std::endl;
}


// return -1 if specified directory is not accessible, otherwise return 0
int mkdirRecursive(const std::string dir, const mode_t mode){
  std::string path = "";
  std::vector<std::string> v;
  boost::split(v, dir, boost::is_any_of("/"));
  for(auto itr = v.begin(); itr != v.end(); ++itr){
    if(itr == v.begin()){
      if(*itr == "~") path = getenv("HOME");
      else if (*itr == ".") path = getenv("PWD");
      else if (*itr == ".."){
        path = getenv("PWD");
        if(path != "/"){
          for(auto ritr = path.rbegin(); *ritr != '/'; path.pop_back(), ++ritr);
          path.pop_back();
        }
      }
      else path = *itr;
    }
    else{
      path += "/" + (*itr);
      int ret = mkdir(path.c_str(), mode);
      if(!((ret == 0) || (ret == EEXIST))) return -1;
    }
  }
  return 0;
}

std::string makefullpath(std::string dir) {
    
  std::string path = "";
  std::vector<std::string> v;
  boost::split(v, dir, boost::is_any_of("/"));
  for(auto itr = v.begin(); itr != v.end(); ++itr){
    if(itr == v.begin()){
      if(*itr == "~") path = getenv("HOME");
      else if (*itr == ".") path = getenv("PWD");
      else if (*itr == ".."){
        path = getenv("PWD");
        if(path != "/"){
          for(auto ritr = path.rbegin(); *ritr != '/'; path.pop_back(), ++ritr);
          path.pop_back();
        }
      }
      else path = *itr;
    }
    else{
      path += "/" + (*itr);
    }
  }
  return path;

}

#endif
