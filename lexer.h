int reloadBuffer(char** buffers,FILE* fp, buffersize buf_size);
int initializeBuffer(char** buffers,FILE* fp,buffersize buf_size);
FILE* getStream(FILE* fp,char** B, buffersize buf_size);
tokenInfo getNextToken(FILE* fp);
