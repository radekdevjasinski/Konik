struct movesList* movesToAnimate = NULL;
struct movesList* movesVisited = NULL;
struct movesList
{
	int moveId;
	int x, y;
	struct movesList* next;
};
struct movesList* create_list(int moveId, int x, int y)
{
	struct movesList* front = (struct movesList*)
		malloc(sizeof(struct movesList));
	if (NULL != front)
	{
		front->moveId = moveId;
		front->x = x;
		front->y = y;
		front->next = NULL;
	}
	return front;
}

struct movesList* insert_front(struct movesList* front,
	struct movesList* new_node)
{
	new_node->next = front;
	return new_node;
}

struct movesList* find_spot(struct movesList* front, int moveId)
{
	struct movesList* prev = NULL;
	while ((NULL != front) && (front->moveId < moveId))
	{
		prev = front;
		front = front->next;
	}
	return prev;
}

void insert_after(struct movesList* node, struct movesList* new_node)
{
	new_node->next = node->next;
	node->next = new_node;
}

void insert_back(struct movesList* back, struct movesList* new_node)
{
	back->next = new_node;
}

struct movesList* insert_node(struct movesList* front, int moveId, int x, int y)
{
	if (NULL == front)
		return NULL;

	struct movesList* new_node = (struct movesList*)
		malloc(sizeof(struct movesList));
	if (NULL != new_node)
	{
		new_node->moveId = moveId;
		new_node->x = x;
		new_node->y = y;
		new_node->next = NULL;
		if (front->moveId >= moveId)
			return insert_front(front, new_node);
		else
		{
			struct movesList* node = find_spot(front, moveId);
			if (NULL != node->next)
				insert_after(node, new_node);
			else
				insert_back(node, new_node);
		}
	}
	return front;
}
void remove_list(struct movesList** front)
{
	struct movesList* next = NULL;
	while (NULL != *front)
	{
		next = (*front)->next;
		free(*front);
		*front = next;
	}
}
void print_list(struct movesList* front)
{
	for (; NULL != front; front = front->next) {
		printf("(%d,%d); ", front->x, front->y);
	}
	printf("\n");
}