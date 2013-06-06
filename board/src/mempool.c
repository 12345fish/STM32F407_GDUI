#include "string.h"
#include "MemPool.h"


static struct mem *ram_end;
static struct mem *lfree; /*ָ����ײ����п��ָ��*/
static u8_t ram[MEM_SIZE+sizeof(struct mem)+MEM_ALIGNMENT];
/******************************************************************************
* ������  : plug_holes()
* ����    : ����־������״̬,�����䵱ǰ״̬��һ�ɸ�λΪDisble
* ����    : time��ʱʱ��(��λ������)
* ���    : None
* ����    : None
* ����    :
* ��������: 02/07/2011 
* �汾    : V1.00
*******************************************************************************/
static void plug_holes(struct mem *mem)
{
    struct mem *nmem;
    struct mem *pmem;

    ASSERT("plug_holes: mem >= ram", (u8_t*)mem >= ram);
    ASSERT("plug_holes: mem < ram_end", (u8_t*)mem < (u8_t*)ram_end);
    ASSERT("plug_holes: mem->used == 0", mem->used == 0);
    /* plug hole forward */
    ASSERT("plug_holes: mem->next <= MEM_SIZE", mem->next <= MEM_SIZE);

    nmem = (struct mem*) &ram[mem->next];
    if (mem != nmem && nmem->used == 0 && (u8_t*)nmem != (u8_t*)ram_end)
    {
        if (lfree == nmem)
        {
            lfree = mem;
        } mem->next = nmem->next;
        ((struct mem*) &ram[nmem->next])->prev = (u8_t*)mem - ram;
    }

    /* plug hole backward */
    pmem = (struct mem*) &ram[mem->prev];
    if (pmem != mem && pmem->used == 0)
    {
        if (lfree == mem)
        {
            lfree = pmem;
        } pmem->next = mem->next;
        ((struct mem*) &ram[mem->next])->prev = (u8_t*)pmem - ram;
    }

}
/******************************************************************************
* ������  : mem_init()
* ����    : ��ʼ���ڴ��
*		  :	ʹ���ڴ��������ͷ��ڴ�ǰ�����ȵ��ô˺�����ʼ���ڴ��!
* ����    : None
* ���    : None
* ����    : None
* ����    :
* ��������: 02/07/2011 
* �汾    : V1.00
*******************************************************************************/
void mem_init(void)
{
    struct mem *mem;

    memset(ram, 0, MEM_SIZE);
    mem = (struct mem*)ram;
    mem->next = MEM_SIZE;
    mem->prev = 0;
    mem->used = 0;
    ram_end = (struct mem*)&ram[MEM_SIZE];
    ram_end->used = 1;
    ram_end->next = MEM_SIZE;
    ram_end->prev = MEM_SIZE;
    lfree = (struct mem*)ram;
}
/******************************************************************************
* ������  : mem_free()
* ����    : �ͷ��ڴ�
* ����    : *rmem:ָ��Ҫ�ͷŵ��ڴ��ָ��
* ���    : None
* ����    : None
* ����    :
* ��������: 02/07/2011 
* �汾    : V1.00
*******************************************************************************/
void mem_free(void *rmem)
{
    struct mem *mem;

    if (rmem == NULL)
    {
        DEBUGF(MEM_DEBUG | DBG_TRACE | 2, ("mem_free(p == NULL) was called.\n"));
        return ;
    }
    ASSERT("mem_free: legal memory", (u8_t*)rmem >= (u8_t*)ram && (u8_t*) rmem < (u8_t*)ram_end);
    
	if ((u8_t*)rmem < (u8_t*)ram || (u8_t*)rmem >= (u8_t*)ram_end)
    {
        DEBUGF(MEM_DEBUG | 3, ("mem_free: illegal memory\n"));
        return ;
    }
    mem = (struct mem*)((u8_t*)rmem - SIZEOF_STRUCT_MEM);
    ASSERT("mem_free: mem->used", mem->used);
    mem->used = 0;
    if (mem < lfree)
    {
        lfree = mem;
    }
    plug_holes(mem);
}
/******************************************************************************
* ������  : *mem_reallocm()
* ����    : ���·����ڴ棬���ҽ����ڴ��е����ݿ������µ��ڴ�����
* ����    : *rmem:Ҫ���·����ڴ��ָ��  newsize:���·�����ڴ�Ĵ�С(��λ:�ֽ�)
* ���    : None
* ����    : nmem:���·���õ��ڴ��ͷ��ַ
* ����    :
* ��������: 02/07/2011 
* �汾    : V1.00
*******************************************************************************/
void *mem_reallocm(void *rmem, mem_size_t newsize)
{
    void *nmem;
    nmem = mem_malloc(newsize);
    if (nmem == NULL)
    {
        return mem_realloc(rmem, newsize);
    }
    memcpy(nmem, rmem, newsize);
    mem_free(rmem);
    return nmem;
}
/******************************************************************************
* ������  : *mem_realloc()
* ����    : ���·����ڴ棬���������ڴ��е����ݿ������µ��ڴ�����
* ����    : *rmem:Ҫ���·����ڴ��ָ��  newsize:���·�����ڴ�Ĵ�С(��λ:�ֽ�)
* ���    : None
* ����    : rmem:���·���õ��ڴ��ͷ��ַ
* ����    :
* ��������: 02/07/2011 
* �汾    : V1.00
*******************************************************************************/
void *mem_realloc(void *rmem, mem_size_t newsize)
{
    mem_size_t size;
    mem_size_t ptr, ptr2;
    struct mem *mem,  *mem2;

    /* Expand the size of the allocated memory region so that we can
    adjust for alignment. */
    if ((newsize % MEM_ALIGNMENT) != 0)
    {
        newsize += MEM_ALIGNMENT - ((newsize + SIZEOF_STRUCT_MEM) % MEM_ALIGNMENT);
    }

    if (newsize > MEM_SIZE)
    {
        return NULL;
    }
	ASSERT("mem_realloc: legal memory",(u8_t*)rmem >= (u8_t*)ram && (u8_t*)rmem < (u8_t*)ram_end);
    if ((u8_t*)rmem < (u8_t*)ram || (u8_t*)rmem >= (u8_t*)ram_end)
    {
        DEBUGF(MEM_DEBUG | 3, ("mem_realloc: illegal memory\n"));
        return rmem;
    }
    mem = (struct mem*)((u8_t*)rmem - SIZEOF_STRUCT_MEM);
    ptr = (u8_t*)mem - ram;
    size = mem->next - ptr - SIZEOF_STRUCT_MEM; 
    if (newsize + SIZEOF_STRUCT_MEM + MIN_SIZE < size)
    {
        ptr2 = ptr + SIZEOF_STRUCT_MEM + newsize;
        mem2 = (struct mem*) &ram[ptr2];
        mem2->used = 0;
        mem2->next = mem->next;
        mem2->prev = ptr;
        mem->next = ptr2;
        if (mem2->next != MEM_SIZE)
        {
            ((struct mem*) &ram[mem2->next])->prev = ptr2;
        }

        plug_holes(mem2);
    }
    return rmem;
}
/******************************************************************************
* ������  : *mem_malloc()
* ����    : �����ڴ�
* ����    : size:��Ҫ�����ڴ�Ĵ�С(��λ:�ֽ�)
* ���    : None
* ����    : ����õ��ڴ��ͷ��ַ
* ����    :
* ��������: 02/07/2011 
* �汾    : V1.00
*******************************************************************************/
void *mem_malloc(mem_size_t size)
{
    mem_size_t ptr, ptr2;
    struct mem *mem,  *mem2;
    if (size == 0)
    {
        return NULL;
    }

    /* Expand the size of the allocated memory region so that we can
    adjust for alignment. */
    if ((size % MEM_ALIGNMENT) != 0)
    {
        size += MEM_ALIGNMENT - ((size + SIZEOF_STRUCT_MEM) % MEM_ALIGNMENT);
    }
    if (size > MEM_SIZE)
    {
        return NULL;
    }
    for (ptr = (u8_t*)lfree - ram; ptr < MEM_SIZE; ptr = ((struct mem*) &ram[ptr])->next)
    {
        mem = (struct mem*) &ram[ptr];
        if (!mem->used && mem->next - (ptr + SIZEOF_STRUCT_MEM) >= size +
            SIZEOF_STRUCT_MEM)
        {
            ptr2 = ptr + SIZEOF_STRUCT_MEM + size;
            mem2 = (struct mem*) &ram[ptr2];
            mem2->prev = ptr;
            mem2->next = mem->next;
            mem->next = ptr2;
            if (mem2->next != MEM_SIZE)
            {
                ((struct mem*) &ram[mem2->next])->prev = ptr2;
            }
            mem2->used = 0;
            mem->used = 1;     
            if (mem == lfree)
            {
                /* Find next free block after mem */
                while (lfree->used && lfree != ram_end)
                {
                    lfree = (struct mem*) &ram[lfree->next];
                }
                ASSERT("mem_malloc: !lfree->used", !lfree->used);
            }
            ASSERT("mem_malloc: allocated memory not above ram_end.",
				   (mem_ptr_t)mem + SIZEOF_STRUCT_MEM + size <= (mem_ptr_t)ram_end);
            ASSERT("mem_malloc: allocated memory properly aligned.", 
                   (unsigned long)((u8_t*)mem + SIZEOF_STRUCT_MEM) % MEM_ALIGNMENT == 0);			 
            return (u8_t*)mem + SIZEOF_STRUCT_MEM;
        }
    }
    DEBUGF(MEM_DEBUG | 2, ("mem_malloc: could not allocate %d bytes\n", (int)size));

    return NULL;
}






