#ifndef  __FREE_H__
#define  __FREE_H__

int Free(node_t **);
int Free_data_str(node_t **);

node_t *AllocateNode(tmpstruct_t, opts_t *);
int AllocateNodeData(node_t **, tmpstruct_t, opts_t *);

#endif
