#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
};

// Fill out this structure
struct HashType
{
    struct RecordType *record;
    struct HashType *next;
};

// Compute the hash function
int hash(int x)
{
    return x % 31;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i;
	struct HashType *curr;

	for (i = 0; i < hashSz; ++i)
	{
		curr = &pHashArray[i];
		if (curr->record != NULL)
		{
			printf("index %d -> ", i);
			while (curr != NULL)
			{
				printf("%d, %c, %d -> ", curr->record->id, curr->record->name, curr->record->order);
				curr = curr->next;
			}
			printf("\n");
		}
	}
}

void insertInHash(struct HashType *pHashArray, struct RecordType *record)
{
	int index = hash(record->id);
	struct HashType *curr = &pHashArray[index];

	if (curr->record == NULL)
	{
		curr->record = record;
		curr->next = NULL;
	}
	else
	{
		while (curr->next != NULL)
		{
			curr = curr->next;
		}
		curr->next = (struct HashType *) malloc(sizeof(struct HashType));
		curr->next->record = record;
		curr->next->next = NULL;
	}
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;
	int i;
	int hashSz = 31;
	struct HashType *pHashArray = (struct HashType *) malloc(sizeof(struct HashType) * hashSz);

	for (i = 0; i < hashSz; ++i)
	{
		pHashArray[i].record = NULL;
		pHashArray[i].next = NULL;
	}

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);

	for (i = 0; i < recordSz; ++i)
	{
		insertInHash(pHashArray, &pRecords[i]);
	}

	displayRecordsInHash(pHashArray, hashSz);

	free(pHashArray);
	free(pRecords);

	return 0;
}
