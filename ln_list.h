#ifndef __LN_LIST_H__
#define __LN_LIST_H__

extern size_t ln_caller(node_t **, const char *, const char *, node_t **, const char *, const char *, opts_t *);
extern int AllocateLinkInfo(node_t **, node_t *);
extern size_t ln_cleanemptylinks(node_t **,  opts_t *);
extern int ln_cleanemptylinksref(node_t **);

#endif
