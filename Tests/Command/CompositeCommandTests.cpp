#include "Command/AndCommand.h"
#include "Command/OrCommand.h"
#include "Command/SemicolonCommand.h"

#include "TestUtils/FakeCommand.h"

#include <gtest/gtest.h>

#include <memory>

namespace
{

TEST(AndCommandTest, RunsRightWhenLeftSucceeds)
{
    auto left = std::make_shared<FakeCommand>(true);
    auto right = std::make_shared<FakeCommand>(true);

    AndCommand cmd(left, right);

    EXPECT_TRUE(cmd.Execute());
    EXPECT_EQ(left->GetExecuteCount(), 1);
    EXPECT_EQ(right->GetExecuteCount(), 1);
}

TEST(AndCommandTest, SkipsRightWhenLeftFails)
{
    auto left = std::make_shared<FakeCommand>(false);
    auto right = std::make_shared<FakeCommand>(true);

    AndCommand cmd(left, right);

    EXPECT_FALSE(cmd.Execute());
    EXPECT_EQ(left->GetExecuteCount(), 1);
    EXPECT_EQ(right->GetExecuteCount(), 0);
}

TEST(OrCommandTest, SkipsRightWhenLeftSucceeds)
{
    auto left = std::make_shared<FakeCommand>(true);
    auto right = std::make_shared<FakeCommand>(true);

    OrCommand cmd(left, right);

    EXPECT_TRUE(cmd.Execute());
    EXPECT_EQ(left->GetExecuteCount(), 1);
    EXPECT_EQ(right->GetExecuteCount(), 0);
}

TEST(OrCommandTest, RunsRightWhenLeftFails)
{
    auto left = std::make_shared<FakeCommand>(false);
    auto right = std::make_shared<FakeCommand>(true);

    OrCommand cmd(left, right);

    EXPECT_TRUE(cmd.Execute());
    EXPECT_EQ(left->GetExecuteCount(), 1);
    EXPECT_EQ(right->GetExecuteCount(), 1);
}

TEST(OrCommandTest, ReturnsFalseWhenBothFail)
{
    auto left = std::make_shared<FakeCommand>(false);
    auto right = std::make_shared<FakeCommand>(false);

    OrCommand cmd(left, right);

    EXPECT_FALSE(cmd.Execute());
}

TEST(SemicolonCommandTest, AlwaysRunsBothRegardlessOfLeftResult)
{
    auto left = std::make_shared<FakeCommand>(false);
    auto right = std::make_shared<FakeCommand>(true);

    SemicolonCommand cmd(left, right);

    EXPECT_TRUE(cmd.Execute());
    EXPECT_EQ(left->GetExecuteCount(), 1);
    EXPECT_EQ(right->GetExecuteCount(), 1);
}

TEST(SemicolonCommandTest, ReturnsRightResultEvenWhenLeftSucceeds)
{
    auto left = std::make_shared<FakeCommand>(true);
    auto right = std::make_shared<FakeCommand>(false);

    SemicolonCommand cmd(left, right);

    EXPECT_FALSE(cmd.Execute());
}

TEST(CompositeCommandTest, NestedCompositesShortCircuitAcrossLevels)
{
    // (falseA && trueB) is a Composite used as OrCommand's left operand: OrCommand must still
    // treat it as an ordinary Command and run its right side since the nested And fails.
    auto falseA = std::make_shared<FakeCommand>(false);
    auto trueB = std::make_shared<FakeCommand>(true);
    auto trueC = std::make_shared<FakeCommand>(true);

    auto nestedAnd = std::make_shared<AndCommand>(falseA, trueB);
    OrCommand cmd(nestedAnd, trueC);

    EXPECT_TRUE(cmd.Execute());
    EXPECT_EQ(falseA->GetExecuteCount(), 1);
    EXPECT_EQ(trueB->GetExecuteCount(), 0);
    EXPECT_EQ(trueC->GetExecuteCount(), 1);
}

}  // namespace
