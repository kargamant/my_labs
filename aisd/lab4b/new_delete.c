
int isLeaf(Node* ptr)
{
	int f=1;
	for(int i=0; i<=ptr->n; i++)
	{
		if(ptr->child[i])
		{
			f=0;
			break;
		}
	}
	return f;
}

int InfoDel(Node* ptr, int pos, int key, int rel)
{
	if(rel==1)
	{
		Item* sec=ptr->info[pos].next;
		free(ptr->info[pos].data);
		ptr->info[pos].data=ptr->info[pos].next->data;
		ptr->info[pos].next=ptr->info[pos].next->next;
		sec->next=NULL;
		free(sec);
		sec=NULL;
		return ERR_OK;
	}
	Item* gr=ptr->info[pos];
	Item* prev=ptr->info[pos];
	int k=0;
	while(gr)
	{
		k++;
		if(k==rel)
		{
			free(gr->data);
			gr->data=NULL;
			prev->next=gr->next;
			gr->next=NULL;
			free(gr);
			gr=NULL;
			return ERR_OK;
		}
		prev=gr;
		gr=gr->next;
	}
	return ERR_NF;
}

int DelNode(Btree* tr, int key, int rel)
{
	Node* ptr=tr->root;
	if(ptr)
	{
		//Looking for a key in this node
		int pos=-1;
		int ci=ptr->n;
		for(int i=0; i<ptr->n; i++)
		{
			if(ptr->keys[i]==key)
			{
				pos=i;
				break;
			}
			else if(ptr->keys[i]>key)
			{
				ci=i;
				break;
			}
		}

		//if it is a leaf and there is a key than simply deleting it.
		if(pos!=-1 && isLeaf(ptr))
		{
			//if there are several versions of a key
			if(ptr->info[pos].next) return InfoDel(ptr, pos, key, rel);
			else
			{
				//if incorrect release was entered
				if(rel!=1) return ERR_IR;
				else
				{
					//simple deletion of a single key from a leaf
					free(ptr->info[pos].data);
					ptr->info[pos].data=NULL;
					for(int i=pos; i<ptr->n-1; i++)
					{
						ptr->keys[pos]=ptr->keys[pos+1];
						ptr->info[pos]=ptr->info[pos+1];
					}
					ptr->n=ptr->n-1;
					return ERR_OK;
				}
			}
		}
		//if ptr is not a leaf
		else if(pos!=-1)
		{
			//deletion in case of multiple versions
			if(ptr->info->next) return InfoDel(ptr, pos, key, rel);
			else if(rel!=1) return ERR_IR;
			else
			{
				//left subtree case
				if(ptr->child[pos]->n>=tr->t)
				{
					Btree* ntr=(Btree*)malloc(sizeof(Btree));
					ntr->t=tr->t;
					ntr->root=ptr->child[pos];
					
					//Finding predseccor
					Node* pk=ntr->root;
					while(pk->child[pk->n])
					{
						pk=pk->child[pk->n];
					}
					int ks=pk->keys[pk->n];
					char* is=strdup(pk->info[pk->n].data);
					DelNode(ntr, ks, 1);
					ptr->keys[pos]=ks;
					free(ptr->info[pos].data);
					ptr->info[pos].data=is;
					free(ntr);
					return ERR_OK;
				}
				//right subtree case
				else if(ptr->child[pos+1]->n>=tr->t)
				{
					Btree* ntr=(Btree*)malloc(sizeof(Btree));
					ntr->t=tr->t;
					ntr->root=ptr->child[pos+1];
					
					//Finding sucseccor
					Node* pk=ntr->root;
					while(pk->child[0])
					{
						pk=pk->child[0];
					}
					int ks=pk->keys[0];
					char* is=strdup(pk->info[0].data);
					DelNode(ntr, ks, 1);
					ptr->keys[pos]=ks;
					free(ptr->info[pos].data);
					ptr->info[pos].data=is;
					free(ntr);
					return ERR_OK;
				}
				//merging case
				else
				{
					Node* y=ptr->child[pos];
					Node* z=ptr->child[pos+1];
					
					//inserting key from ptr and key from z into y
					y->keys[y->n]=ptr->keys[pos];
					y->info[y->n].data=strdup(ptr->info[pos].data);
					y->n=y->n+1;

					y->child[y->n]=z->child[0];
					y->keys[y->n]=z->keys[0];
					y->info[y->n].data=strdup(z->info[0].data);
					y->info[y->n].next=z->info[0].next;
					y->n=y->n+1;

					y->child[y->n]=z->child[1];
					
					free(ptr->info[pos].data);
					free(z->info[0].data);
					
					//deleting key
					for(int i=pos; i<ptr->n; i++)
					{
						ptr->keys[i]=ptr->keys[i+1];
						ptr->info[i]=ptr->info[i+1];
					}
					free(z->keys);
					free(z->info);
					free(z->child);
					free(z);
					z=NULL;

					//deleting z pointer
					for(int i=pos+1; i<ptr->n; i++)
					{
						ptr->child[i]=ptr->child[i+1];
					}
					Btree* ntr=(Btree*)malloc(sizeof(Btree));
					ntr->t=tr->t;
					ntr->root=y;
					int res=DelNode(ntr, key, rel);
					return res;
				}
			}
		}
		//if key not in this node
		else
		{
			if(ptr->child[ci]->n>=tr->t)
			{
				Btree* ntr=(Btree*)malloc(sizeof(Btree));
				ntr->t=tr->t;
				ntr->root=ptr->child[ci];
				int res=DelNode(ntr, key, rel);
				free(ntr);
				return res;
			}
			else
			{
				if(ci-1>=0)
				{
					if(ptr->child[ci-1]->n>=tr->t)
					{
						Node* y=ptr->child[ci-1];
						Node* z=ptr->child[ci];


					}
				}
			}
		}
	}
}














