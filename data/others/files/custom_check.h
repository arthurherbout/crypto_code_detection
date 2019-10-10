/*
 Copyright 2014-2017 Thumbtack, Inc.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

         http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/


#ifndef CUSTOM_CHECK_H_
#define CUSTOM_CHECK_H_


typedef struct custom_check {
    char *query;
    char *expected_result;
    char *relop;
} *custom_check_t;

#define CUSTOM_CHECK_QUERY(X) (X->query)
#define CUSTOM_CHECK_RESULT(X) (X->expected_result)
#define CUSTOM_CHECK_OPERATOR(X) (X->relop)

extern custom_check_t custom_check_create(char *query, char *op,
					  char *result);
extern void custom_check_set_query(custom_check_t check, char *query);
extern void custom_check_set_result(custom_check_t check, char *result);
extern void custom_check_set_operator(custom_check_t check, char *relop);
extern void custom_check_destroy(custom_check_t check);


#endif				/* CUSTOM_CHECK_H_ */
