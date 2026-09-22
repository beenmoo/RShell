#include "Core/Executor.h"

#include "TestUtils/FakeCommand.h"

#include <gtest/gtest.h>

#include <memory>

namespace
{

TEST(ExecutorTest, ReturnsRootsResultOnSuccess)
{
    Executor executor;

    EXPECT_TRUE(executor.Execute(std::make_shared<FakeCommand>(true)));
}

TEST(ExecutorTest, ReturnsRootsResultOnFailure)
{
    Executor executor;

    EXPECT_FALSE(executor.Execute(std::make_shared<FakeCommand>(false)));
}

}  // namespace
