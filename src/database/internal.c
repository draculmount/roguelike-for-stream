#include "internal.h"

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

struct Table* lookup_table_impl(struct Tables* tables, char* name)
{
	for(int i=0; i<tables->number_of_tables; i++) {
		if(strcmp(tables->tables[i].name, name) == 0) {
			return &(tables->tables[i]);
		}
	}

	return NULL;
}

struct Table* lookup_table(struct Database_Handle dbh, char* name) {
	return lookup_table_impl(dbh.tables, name);
}

struct Table* lookup_virtual_table(struct Database_Handle dbh, char* name) {
	return lookup_table_impl(dbh.virtual_tables, name);
}

void* get_ptr_column(struct Table* table, size_t row, size_t i)
{
	return table->columns[i].data_begin
		  + row * table->columns[i].type.size *
		  table->columns[i].count;
}

struct Column* lookup_column_impl(struct Table* table, char* column_name)
{
	for(int i=0; i< table->number_of_columns; i++) {
		if(strcmp(table->columns[i].name, column_name) == 0) {
			return &(table->columns[i]);
		}
	}

	return NULL;
}

struct Column* lookup_column(struct Database_Handle dbh, char* table_name, char* column_name)
{
	return lookup_column_impl(
		lookup_table(dbh, table_name),
		column_name
	);
}

void create_table_impl(struct Tables* tables, char* name, int num, va_list args)
{
	struct Table new_table = {
		.name = name,
		.number_of_rows = 0,
		.rows_allocated = 255,
		.datalayout = DATALAYOUT_ROW_ORIENTED
	};

	tables->tables[tables->number_of_tables] = new_table;

	for(int i=0; i<num; i++) {
		struct Column* current_column = &(tables
			->tables[tables->number_of_tables]
			.columns[i]);

		*current_column = va_arg(args, struct Column);

		void *data = calloc(current_column->type.size, 255);

		current_column->data_begin = data;
	}

	tables->tables[tables->number_of_tables]
			.number_of_columns = num;

	tables->number_of_tables++;
}

void create_virtual_table(struct Database_Handle dbh, char* name, int num, ...)
{
	assert(dbh.tables != NULL);
	assert(dbh.tables->number_of_tables < 255);

	va_list args;
	va_start(args, num);
	create_table_impl(dbh.virtual_tables, name, num, args);
	va_end(args);
}

void destory_table(struct Database_Handle dbh, char* name)
{
	for(size_t i_t=0; i_t<dbh.tables->number_of_tables; i_t++) {
		if(strcmp(dbh.tables->tables[i_t].name, name) == 0) {
			// TODO (#34): Create a method for destorying a table 
		}
	}
}