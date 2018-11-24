#include "logging.h"
#include <fstream>

FILE *Logger::logger = fopen("logger.txt","w");

void Logger::LogEvent(const char *event){
   fprintf(Logger::logger, "%s\n", event);
}

void Logger::Finalize(){
   fclose(Logger::logger);
}

DataFlowException::DataFlowException(const char *type, const char *error){
   sprintf(this->msg, "Throwing exception: (%s): ", type);
   //   strcpy(this->msg, type);
   strcat(this->msg, error);
   Logger::LogEvent(this->msg);
}
//
//DataFlowException::~DataFlowException(){
//   delete msg;
//}
