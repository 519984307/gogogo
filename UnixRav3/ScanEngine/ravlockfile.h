#ifndef RAVLOCKFILE_H
#define RAVLOCKFILE_H

int ravlockfile(const char *szLockFile,bool bBlock=true);
void ravunlockfile(int fd);

#endif // RAVLOCKFILE_H
