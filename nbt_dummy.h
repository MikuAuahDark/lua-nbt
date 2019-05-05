/*
 * nbt_dummy.h
 * Documentation header file.
 **/

#include <stdint.h>
#include <stdlib.h>

namespace nbt
{

/** Number datatype that indicates any floating point will be truncated */
typedef int64_t integer;
/** Number datatype that preserve floating point */
typedef double number;
/** Datatype that indicate Lua string */
typedef char *string;
/** Datatype that indicate Lua table */
typedef void *table;
/** Datatype that indicate any Lua type but not nil */
typedef ptrdiff_t any;

/** These enum values are exposed directly in `nbt` table. The enum value is number */
typedef enum TagType {
	/** End of TAG_COMPOUND mark. */
	TAG_END,
	/** Signed byte/int8_t value tag. */
	TAG_BYTE,
	/** Signed short/int16_t value tag. */
	TAG_SHORT,
	/** Signed integer/int32_t value tag. */
	TAG_INT,
	/** Signed long integer/int64_t value tag. */
	TAG_LONG,
	/** Single precision floating-point value tag. */
	TAG_FLOAT,
	/** Double precision floating-point value tag. */
	TAG_DOUBLE,
	/** Array of signed byte/int8_t[] tags. */
	TAG_BYTE_ARRAY,
	/** UTF-8 string value tag. */
	TAG_STRING,
	/** Array of specific tags. */
	TAG_LIST,
	/** Key-value pairs of tags. */
	TAG_COMPOUND,
	/** Array of signed integer/int32_t[] tags. */
	TAG_INT_ARRAY,
	/** Array of signed long integer/int64_t[] tags. */
	TAG_LONG_ARRAY
} TagType;

/** Valid preserve mode for decode(). The enum value is string. */
typedef enum PreserveMode
{
	/** `"tag"` - All value in a table is type of Tag. */
	tag,
	/** `"plain"` - All value in a table is plain Lua type. */
	plain
} PreserveMode;

/** NBT Tag class */
class Tag
{
public:
	/** Get type ID of current NBT tag. */
	TagType getTypeID();
	/** Get the tag name. May be empty string or `nil`. */
	string getName();
	/**
	 * Get string representation of the NBT value.
	 * \exception error thrown if the type is ::TAG_COMPOUND, ::TAG_LIST, ::TAG_BYTE_ARRAY, ::TAG_INT_ARRAY, or ::TAG_LONG_ARRAY.*/
	string getString();
	/**
	 * Get number representation of the NBT value.
	 * \exception error thrown if the value is not convertible to number.
	 **/
	number getNumber();
	/**
	 * Get number representation of the NBT value.
	 * \exception error thrown if the value is not convertible to number.
	 * \warning This truncates the fraction part of the value if it's convertible to number.
	 **/
	integer getInteger();
	/**
	 * Get the value of the NBT tag.
	 * 
	 * - Number is returned for ::TAG_BYTE, ::TAG_SHORT, ::TAG_INT, ::TAG_LONG, ::TAG_FLOAT, and
	 *   ::TAG_DOUBLE type.
	 * - String is returned for ::TAG_STRING type.
	 * - Table is returned for ::TAG_LIST, ::TAG_COMPOUND (key-value pairs), ::TAG_BYTE_ARRAY,
	 *   ::TAG_INT_ARRAY, and ::TAG_LONG_ARRAY type.
	 **/
	any getValue();
	/**
	 * Copy the current tag and return new one with same value(s).
	 * \param shallow Only copy the upper values for array? By default this function will do deep copy.
	 **/
	Tag *copy(bool shallow = false);
	/**
	 * Encode the current NBT tag to its binary representation.
	 * \param noprefix Internal use. Always omit this when calling this function.
	 * \warning Care must be taken when calling this for type other than ::TAG_COMPOUND.
	 *          Some NBT parsers only decodes NBT data if it's begin with ::TAG_COMPOUND.
	 *          This library is not one of them.
	 **/
	string encode(bool noprefix = false);
	/**
	 * Dump the current NBT tag to human readable pretty-printed string representation.
	 * \note The dump format follows [PyNBT](https://github.com/TkTech/PyNBT) output.
	 **/
	operator string();
private:
	Tag(integer type, any value, string name = "");
	integer _type;
	string _name;
	any _value;
};

/**
 * Input reader function signature.
 * 
 * When function is supplied to nbt::decode(inputReader, string) these things must be considered:
 * - Returns uncompressed data.
 * - Returns at least \p minread length of binary-string. If it returns less than that, the decoding
 *   function will fail. Function is allowed to return size bigger than \p minread.
 * - If error occured, return `nil` AND the reason in string (2 values).
 * 
 * \param minread Minimum string that is needed by the decoder.
 * \returns Uncompressed binary-string of the next NBT data needed.
 * */
typedef string(*inputReader)(integer minread);

/** Create new ::TAG_BYTE NBT tag value. */
Tag newByte(integer value, string name = "");
/** Create new ::TAG_SHORT NBT tag value. */
Tag newShort(integer value, string name = "");
/** Create new ::TAG_INT NBT tag value. */
Tag newInt(integer value, string name = "");
/**
 * Create new ::TAG_LONG NBT tag value.
 * \warning Due to pure Lua can't handle anything 2^53, only use this if really necessary.
 **/
Tag newLong(integer value, string name = "");
/** Create new ::TAG_FLOAT NBT tag value. */
Tag newFloat(number value, string name = "");
/** Create new ::TAG_DOUBLE NBT tag value. */
Tag newDouble(number value, string name = "");
/**
 * Create new ::TAG_BYTE_ARRAY NBT tag value.
 * \exception error thrown if the array table contains non-number.
 **/
Tag newByteArray(table value, string name = "");
/** Create new ::TAG_BYTE_ARRAY NBT tag value. */
Tag newString(string value, string name = "");
/**
 * Create new ::TAG_LIST NBT tag with specified \p typeID.
 * \exception error thrown if non-matching Tag object is passed.
 **/
Tag newList(TagType typeID, table value, string name = "");
/**
 * Create new ::TAG_COMPOUND NBT tag.
 * \warning This function does not support automatic Tag type conversion, so the value must be a
 *          type of Tag.
 * \exception error thrown if it contains value other than Tag type.
 * \code{.lua}
 * -- Example of creating TAG_COMPOUND
 * local nbt = require("nbt")
 * tagCompound = nbt.newCompound({
 *     hello = nbt.newString("World"),
 *     integerValue = nbt.newInt(12345),
 *     doublePrecision = nbt.newDouble(math.pi),
 *     list = nbt.newList(nbt.TAG_STRING, {"bandori", "bang", "dream", "livesim2", "over the rainbow"})
 * })
 * \endcode
 **/
Tag newCompound(table value, string name = "");
/**
 * Create new ::TAG_INT_ARRAY NBT tag value.
 * \exception error thrown if the array table contains non-number.
 **/
Tag newIntArray(table value, string name = "");
/**
 * Create new ::TAG_LONG_ARRAY NBT tag value.
 * \warning Due to pure Lua can't handle anything 2^53, only use this if really necessary.
 * \exception error thrown if the array table contains non-number.
 **/
Tag newLongArray(table value, string name = "");
/**
 * Decode \p input binary-string to NBT Tag. This library accepts NBT tag not begin with TAG_COMPOUND.
 * \param input Uncompressed NBT data binary-string to be decoded.
 * \param preservemode How this function returns the values? (note that this value is string name of
 *        the enum value).
 * \error thrown if \p input is not a valid NBT data.
 **/
Tag decode(string input, PreserveMode preservemode = PreserveMode::tag);
/**
 * Decode an NBT data based on specified \p reader function. This library accepts NBT tag not begin
 * with TAG_COMPOUND.
 * \param reader Reader function.
 * \param preservemode How this function returns the values? (note that this value is string name of
 *        the enum value).
 * \error thrown if \p input is not a valid NBT data or there's error in \p reader function.
 **/
Tag decode(inputReader reader, PreserveMode preservemode = PreserveMode::tag);

}
