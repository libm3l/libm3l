#include "format_type.h"
#include "internal_format_type.h"
#include "Set_Default_Parameters.h"

void m3l_set_Add(opts_t **Popts){

	(*Popts)->opt_i = '\0'; 
	(*Popts)->opt_d = '\0'; 
	(*Popts)->opt_f = '\0'; 
	(*Popts)->opt_r = 'r'; 
	(*Popts)->opt_I = '\0'; 
	(*Popts)->opt_k = '\0'; 
	(*Popts)->opt_b = '\0';
	(*Popts)->opt_l = '\0';
}

void m3l_set_Cat(opts_t **Popts){

	(*Popts)->opt_s = '0';
	(*Popts)->opt_p = '0'; 
	(*Popts)->opt_d = '0'; 
	(*Popts)->opt_f = '0';
	(*Popts)->opt_l = '0';
	(*Popts)->opt_L = '0';
}

void m3l_set_Cp(opts_t **Popts){

	(*Popts)->opt_i = '\0'; 
	(*Popts)->opt_d = '\0'; 
	(*Popts)->opt_f = '\0'; 
	(*Popts)->opt_r = 'r'; 
	(*Popts)->opt_I = '\0'; 
	(*Popts)->opt_k = '\0'; 
	(*Popts)->opt_b = '\0';
	(*Popts)->opt_l = '\0';
	(*Popts)->opt_L = '\0'; 
	(*Popts)->opt_nomalloc = '\0'; // if 'm', do not malloc (used in Mklist --no_malloc
	(*Popts)->opt_add = '\0';
}


void m3l_set_Detach_List(opts_t **Popts){

	(*Popts)->opt_i = '\0'; 
	(*Popts)->opt_d = '\0'; 
	(*Popts)->opt_f = '\0'; 
	(*Popts)->opt_r = 'r'; 
	(*Popts)->opt_I = '\0'; 
	(*Popts)->opt_L = '\0'; 
	(*Popts)->opt_l = '\0';
}

void m3l_set_Find(opts_t **Popts){

	(*Popts)->opt_i = '\0'; 
	(*Popts)->opt_d = '\0'; 
	(*Popts)->opt_f = '\0'; 
	(*Popts)->opt_r = '\0'; 
	(*Popts)->opt_I = '\0';
	(*Popts)->opt_d = '\0'; 
	(*Popts)->opt_f = '\0'; 
	(*Popts)->opt_l = '\0';

}

void m3l_set_Fread(opts_t **Popts){

	(*Popts)->opt_linkscleanemptlinks = '\0';  // clean empty links
	(*Popts)->opt_nomalloc = '\0'; // if 'm', do not malloc (used in Mklist --no_malloc
	(*Popts)->opt_linkscleanemptrefs = '\0'; // clean empty link references
	
}


void m3l_set_Fwrite(opts_t **Popts){
	
	(*Popts)->opt_linkscleanemptlinks = '\0';  // clean empty links
	(*Popts)->opt_nomalloc = '\0'; // if 'm', do not malloc (used in Mklist --no_malloc

}

void m3l_set_Send_to_tcpipsocket(opts_t **Popts){
	
	(*Popts)->opt_linkscleanemptlinks = '\0';  // clean empty links
	(*Popts)->opt_nomalloc = '\0'; // if 'm', do not malloc (used in Mklist --no_malloc
	(*Popts)->opt_linkscleanemptrefs = '\0'; // clean empty link references
	(*Popts)->opt_tcpencoding = 'I'; // serialization and encoding when sending over TCP/IP
	(*Popts)->opt_MEMCP = 'S';  // type of buffering
	(*Popts)->opt_EOBseq = '\0'; // send EOFbuff sequence only
}


void m3l_set_Send_receive_tcpipsocket(opts_t **Popts){

	(*Popts)->opt_linkscleanemptlinks = '\0';  // clean empty links
	(*Popts)->opt_nomalloc = '\0'; // if 'm', do not malloc (used in Mklist --no_malloc
	(*Popts)->opt_linkscleanemptrefs = '\0'; // clean empty link references
	(*Popts)->opt_tcpencoding = 'I'; // serialization and encoding when sending over TCP/IP
	(*Popts)->opt_shutdown = '\0'; // shutdown when finished with sending
	(*Popts)->opt_MEMCP = 'S';  // type of buffering
	(*Popts)->opt_EOBseq = '\0'; // send EOFbuff sequence only
	(*Popts)->opt_REOBseq = '\0'; // read EOFbuff sequence only
}

void m3l_set_Receive_send_tcpipsocket(opts_t **Popts){
	
 	(*Popts)->opt_linkscleanemptlinks = '\0';  // clean empty links
	(*Popts)->opt_nomalloc = '\0'; // if 'm', do not malloc (used in Mklist --no_malloc
	(*Popts)->opt_linkscleanemptrefs = '\0'; // clean empty link references
	(*Popts)->opt_tcpencoding = 'I'; // serialization and encoding when sending over TCP/IP
	(*Popts)->opt_shutdown = '\0'; // shutdown when done with receiving
	(*Popts)->opt_MEMCP = 'S';  // type of buffering
	(*Popts)->opt_EOBseq = '\0'; // send EOFbuff sequence only
	(*Popts)->opt_REOBseq = '\0'; // read EOFbuff sequence only
	
}

void m3l_set_Receive_tcpipsocket(opts_t **Popts){

	(*Popts)->opt_linkscleanemptlinks = '\0';  // clean empty links
	(*Popts)->opt_nomalloc = '\0'; // if 'm', do not malloc (used in Mklist --no_malloc
	(*Popts)->opt_tcpencoding = 'I'; // serialization and encoding when sending over TCP/IP
	(*Popts)->opt_MEMCP = 'S';  // type of buffering
	(*Popts)->opt_REOBseq = '\0'; // read EOFbuff sequence only
}

void m3l_set_MkTCPIPHeader(opts_t **Popts){
	
/*
 * options for add and remove list
 */
	(*Popts)->opt_i = '\0';
	(*Popts)->opt_d = '\0'; 
	(*Popts)->opt_f = '\0'; 
	(*Popts)->opt_r = 'r'; 
	(*Popts)->opt_I = '\0'; 
	(*Popts)->opt_k = '\0'; 
	(*Popts)->opt_b = '\0';
	(*Popts)->opt_l = '\0';
/*
 * options for allocate list
 */	
	(*Popts)->opt_n = '\0'; 
	(*Popts)->opt_b = '\0'; 
	(*Popts)->opt_nomalloc = '\0';	
}