#include <unittest-c++/UnitTestC++.h>

#include <iostream>
#include "parse-c++/CodeGenerator.h"
#include "parse-c++/TestData.h"

using namespace std;

namespace CPPParser {
namespace Test {

class ASTBuildTest
    : public ::UnitTestCpp::TestFixture
{
protected:
    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }
};

TEST_FIXTURE(ASTBuildTest, EmptyAST)
{
    AST ast;

    EXPECT_FALSE(ast.IsValid());
}

} // namespace Test
} // namespace CPPASTVisitor
