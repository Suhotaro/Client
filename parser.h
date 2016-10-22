#ifndef __PARSER_H__
#define __PARSER_H__

#include <iostream>
#include <string>
#include "util.h"
#include <fstream>

/* TODO: Find better error handling mechanism */
typedef enum
{
	PARSER_ERROR_NONE = 0,
	PARSER_ERROR_INIT_FALSE,
	PARSER_ERROR_NO_SRC_FILE,
	PARSER_ERROR_EOF,
} parser_error;

class Parser
{
public:
    Parser() {}
    ~Parser() {}

    /**
     * @brief Initializes a parser
     * @returns PARSER_ERROR_NONE on success, any other error on filure
     */
    virtual parser_error init() = 0;

    /**
	 * @brief Parses input to get an intervals one by one
	 * @detailed Asume that it was given a file which contains three intervals
	 * it means that a one has to invoke the "process "method three times to
	 * get each interval.
     * @param[in out] low : interval starts from
     * @param[in out] high : interval ends in
	 * @returns PARSER_ERROR_NONE on success, PARSER_ERROR_EOF if file has been
	 * red till the end, any other error on filure.
	 */
    virtual parser_error process(int &low, int &high) = 0;

    /**
     * @brief Finilizes a parser
     * @returns PARSER_ERROR_NONE on success, any other error on filure
     */
    virtual void deinit() = 0;

    /**
     * @brief base setter
     */
    virtual void set_file_name(std::string fname) = 0;

    /**
	 * @brief base getter
	 */
    virtual std::string get_file_name() =0;
};

/* TODO: Has to try to use fabric template right here */
class XMLParser final : public Parser
{
private:
	std::string file_name;
    std::ifstream in;

public:
	XMLParser (const std::string fname) : file_name(fname) {}
	~XMLParser() {}

    void set_file_name(std::string fname) final { file_name = fname; }
    std::string get_file_name() final { return file_name; }

    parser_error init() final;
    parser_error process(int &low, int &high) final;
	void deinit() final;
};

#endif //__PARSER_H__
