#ifndef  __FREE_H__
#define  __FREE_H__

int Free(node_t **);
int Free_data_str(node_t **);

int Allocate(node_t **, tmpstruct_t );

node_t *AllocateNode(tmpstruct_t);
int AllocateNodeData(node_t **, tmpstruct_t);

#endif
