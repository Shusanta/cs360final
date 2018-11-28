#include "type.h"
#include "util.c"


char * t1 = 'xwrxwrxwr-------';
char * t2 = '----------------';



void init() // when the filesystem starts, we initialize all global data structures and P0 of super user and are assigned that way.
{
    
    int i,j;

    for( i = 0; i < NMINODE; i++){ // initialize all minnodes as free
        minode[i].refCount = 0;
    }

    for( i = 0; i< NPROC; i++){ // initialize procs
        proc[i].cwd = 0;
    }
    proc[0].uid = 0;

    proc[1].uid = 1;

    proc[0].pid = 1;

    proc[1].pid = 2;

    running = &proc[0]; // the first process runs first
    
}

void mount_root(char * rootdev){


    dev = open(rootdev, O_RDWR); 

    char buf[1024];

    if (dev < 0){
        printf("CANT OPEN ROOT DEVICEEE\n");
    }

    get_block(dev,SUPERBLOCK,buf); // get the superblock

    sp = (SUPER *) buf;
    if(sp->s_magic != SUPER_MAGIC){
        printf("Not an EXT2 FILESYSTEM\n");
        exit(0);
    }

    ninodes = sp->s_inodes_count;
    nblocks = sp->s_blocks_count;


    // next get group descriptor to get the bitmap and inodemap to see what data is free 
    get_block(dev,GDBLOCK,buf);

    //in the group descriptor get the bitmap and impa and iblock
    gp = (GD*)buf;
    bmap = gp->bg_block_bitmap;
    imap = gp->bg_inode_bitmap;
    iblock  = gp->bg_inode_table;

    printf("bmap=%d imap=%d iblock=%d\n", bmap, imap, iblock);

    root = iget(dev,2); // gets the root inode
}



