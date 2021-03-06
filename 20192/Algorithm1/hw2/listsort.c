// insertion, merge sorting using linkedlist
/* Directions: complete the code by filling in "FILL" parts
 *   adding functions and variables is allowed
 *   do not change print and file load/save
 *   do not use your compiler-specific libraries
 * FILL parts: insertion and merge sort on linked lists
 */

/* Insertion sort and merge sort on Linked Lists
 * COMP319 Algorithms, Fall 2019
 * School of Electronics Engineering, Kyungpook National University
 * Instructor: Gil-Jin Jang
 */

#include <stdio.h>
#include <stdlib.h>
long *readkeys_textfile( const char infile[], int *pN )
  // returns an array of long integers, with its size stored in
  // the memory indicated by integer pointer variable pN
  // the retured memory should freed by the caller
{
  long *A, tmp;
  int i;
  FILE *fp;

  // check for input file name
  if ( infile == NULL ) {
    fprintf(stderr, "NULL file name\n");
    return NULL;
  }

  // check for file existence
  fp = fopen(infile,"r");
  if ( !fp ) {
    fprintf(stderr, "cannot open file %s\n",infile);
    return NULL;
  }
  else {
    // check for number of elements
    if ( fscanf(fp, "%d", pN) != 1 || *pN <= 0 ) {
      fprintf(stderr, "cannot read number of elements %s\n",infile);
      return NULL;
    }
    else {
      A = (long*)malloc(sizeof(long)*(*pN));
      for (i=0; i<(*pN); i++) {
	if ( fscanf(fp, "%ld", &tmp) != 1 ) {
	  fprintf(stderr, "cannot read value at %d/%d\n",i+1,(*pN));
	  *pN = i;	// read data items
	  return A;
	}
	else A[i] = tmp;
      }
    }
    fclose(fp);
    return A;
  }
}

struct LNode {	// update: item type chagned to long, variable name to key
  long key;
  struct LNode *next;
};

struct LNode *generateLNode(long key)
{
  struct LNode *tmp;
  tmp = (struct LNode*) malloc(sizeof(struct LNode));
  tmp->key = key;
  tmp->next = NULL;	// bugfix on 190924
  return tmp;
}

struct LNode *insert_head(struct LNode *head, struct LNode *newPtr)
{
  if ( newPtr == NULL ) return head;	// Nothing to add
  else {

    newPtr->next = head;
    return newPtr;
  }
}

struct LNode *insert_next(struct LNode *prev, struct LNode *newPtr)
{
  //struct LNode *cur;	// unnecessary 190924
  if ( newPtr == NULL ) return prev;	// Nothing to add
  else if ( prev == NULL ) return insert_head(NULL, newPtr);
  else {
    //cur = prev->next;		// unnecessary, so removed, on 190924
    //newPtr->next = cur;	// unnecessary, so removed, on 190924
    newPtr->next = prev->next;
    prev->next=newPtr;
    return newPtr;
  }
}

void traverse(struct LNode *L)
{
  struct LNode *cur;
  printf("[");
  for (cur=L ; cur != NULL ; cur = cur->next )
    printf("%ld ",cur->key);
  printf("]\n");
}

void free_list_iterative (struct LNode *L)
{
  struct LNode *tmp;
  while ( L != NULL ) {
    tmp = L;
    L = L->next;	// every node access should be done before free()
    free(tmp);
  }
}

// read a linked list from textfile
struct LNode *array2linkedlist(long A[], int N)
{
  struct LNode *head, *cur, *prev;
  int i;

  head = NULL; prev = NULL;
  for (i=0; i<N; i++) {
    cur = generateLNode(A[i]);
    prev = insert_next(prev,cur);
    if ( head == NULL ) head = prev;
  }

  return head;
}

struct LNode *readkeys_textfile_linkedlist( const char infile[], int *pN )
  // read text file of integers:
  // number_of_intergers integer1 integer2 ...
  // then convert it to a linked list
  // returns a list list of struct LNode, with its size stored in
  // the memory indicated by integer pointer variable pN
{
  struct LNode *head;
  long *A;

  A = readkeys_textfile(infile, pN); //array로 만들어줌
  if ( A != NULL && *pN > 0 ) {
    head = array2linkedlist(A,*pN); // array를 linkedlist로
    free(A);                        // array를 free함
    return head;
  }
  else return NULL;	// failed
}

void savekeys_textfile_linkedlist( const char outfile[],
    struct LNode *head, int N )
  // save the given linked list to file whose name given by outfile[]
{
  FILE *fp;
  // check for output filename
  if ( outfile == NULL ) {
    fprintf(stderr, "NULL file name\n");
    return;
  }

  // check for file existence
  fp = fopen(outfile,"w");
  if ( !fp ) {
    fprintf(stderr, "cannot open file for write %s\n",outfile);
  }
  else {
    // save number of elements
    fprintf(fp, "%d\n",N);
    for (; head; head = head->next ) fprintf(fp, "%ld ", head->key);
    fprintf(fp,"\n");
    fclose(fp);
  }
}
/////////////////////////////////////////////////////////////
// linked list insertion sort
/////////////////////////////////////////////////////////////
/* GJ: may add any additional functions */
struct LNode *find_min(struct LNode *ptr, int method){
      long low=10000000;

      struct LNode *tmp, *tmp2=NULL, *min, *prev;
      for(tmp=ptr ; tmp!=NULL ; tmp=tmp->next){
            if(tmp->key < low){
                  min = tmp;
                  low = tmp->key;
                  prev = tmp2;
            }
            tmp2 = tmp;
      }

      if(min->key!=ptr->key && method==1){
            prev->next = min->next;
      }
      return min;
}

struct LNode *list_insertion_sort (struct LNode *ptr, int size,int method ){
      struct LNode *head, *prev, *tmp;
      int i;
      head = prev = NULL;
      for(i=0 ; i<size ; i++){
            tmp = find_min(ptr,method);
            if(tmp == ptr){
                  ptr = ptr->next;
            }
            prev=insert_next(prev,tmp);
            if(head==NULL)  head = prev;
      }
      return head;
}




// linked list merge sort
struct LNode *list_array_merge_sort( struct LNode **head_array, int N,int method)
{
      int i, j=N-1, k, size, cnt=2, set;
      struct LNode *min1, *min2, *tmp, *prev, *tmp2;

      size=(N+1)/2;
      while(size>0){
            for(i=0 ; i<size ; i++){
                        if(i>=j)    set=1;
                        tmp = prev = NULL;
                  for(k=0 ; k<cnt ; k++){
                        min1=min2=NULL;
                        if(head_array[i]==NULL || head_array[j]==NULL){
                              break;
                        }
                        min1 = find_min(head_array[i],method);
                        min2 = find_min(head_array[j],method);
                        if(min1->key < min2->key){
                              if(min1->key == head_array[i]->key){
                                    head_array[i] = head_array[i]->next;
                              }
                              prev=insert_next(prev,min1);
                        }
                        else{
                               if(min2->key == head_array[j]->key){
                                    head_array[j] = head_array[j]->next;
                               }
                              prev=insert_next(prev,min2);
                        }
                        if(tmp==NULL)
                              tmp = prev;
                  }

                  for(tmp2=tmp ; tmp2!=NULL ; tmp2=tmp2->next){
                        if(tmp2->next==NULL)
                              break;
                  }
                  if(head_array[i]==NULL){
                        tmp2->next = head_array[j];

                  }else if(head_array[j]==NULL){
                        tmp2->next = head_array[i];
                  }
                  head_array[i]=tmp;
                  if(set==1){
                        set=0;
                  }else{
                        j--;
                  }
            }
            if(size>1){
                  size = (size+1)/2;
            }else{
                  size=0;
            }
            cnt *=2;
      }
      return head_array[0];
  /* GJ: FILL */
}

/////////////////////////////////////////////////////////////

/* File name by Argument input -> scanf read
 * Input file name can be typed in the terminal
 * Update on 9/24: element size is changed to long,
 * to distinguish array index easily
 * in struct Node, item -> key, Node -> LNode
 * Update on 9/25: file reading is moved to a function readkeys_textfile()
 */
int main()
{
  int i, numElements;
  struct LNode *head;

  /* for file name, max length 1023 including path */
  char infile[1024], outfile[1024];
  int method;	// which sorting method37
  // required for mergesort
  struct LNode **H;
  /* file name given by keyboard */
  printf("Input file name? ");
  scanf("%s",infile);
  printf("Output file name? ");
  scanf("%s",outfile);
  
  printf("Method (1: insertion, 2: merge)? ");
  scanf("%d",&method);

  /* read text file of integers:
   * number_of_intergers integer1 integer2 ...
   * then convert it to a linked list */
  head = readkeys_textfile_linkedlist(infile, &numElements);

  if ( head != NULL ) {
    // print all if not too many
    if ( numElements < 100 )	
    {
      traverse(head);
      printf("%d Loaded\n", numElements);
    }

    switch ( method ) {
      case 1:	// insertion sort
	head = list_insertion_sort(head, numElements, method);
	break;
      case 2:	// merge sort
	// change linked list to individual list items,
	// and save their pointers to an array of LNode* types
	H = (struct LNode**) malloc(sizeof(struct LNode*)*numElements);
	for (i=0; i<numElements; i++, head=head->next) H[i] = head;

	// have to isolate list items later
	for (i=0; i<numElements; i++) H[i]->next=NULL;

	head = list_array_merge_sort(H,numElements,method);
	break;
      default:
	break;
    }

    // print out results, if not too many
    if ( numElements < 100 )	// GJ: comment out this line to always display
      traverse(head);

    // save results
    savekeys_textfile_linkedlist(outfile, head, numElements);

    // once allocated, should be freed after use
    free_list_iterative(head);
  }
  return 0;
}
