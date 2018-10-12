#include <fstream>
#include <memory>
#include <catch.hpp>
#include <pybind11/pybind11.h>

#include "kgekit.h"
#include "entity_number_indexer.h"

namespace kgekit
{

using namespace Catch;
using namespace std;

class IndexerTestsFixture {
protected:
    EntityNumberIndexer getEntityNumberIndexer()
    {
        py::list content;
        if (content.size() == 0) {
            std::ifstream f(path);

            std::string h, r, t;
            while (f >> h >> r >> t) {
                content.append(Triple({h, r, t}));
            }
        }
        return EntityNumberIndexer(content, "hrt");
    }
    std::string path = "../../tests/fixtures/triple.txt";
    // static py::list content;
};

TEST_CASE_METHOD(IndexerTestsFixture, "build index lazyly", "[indexing]")
{
    auto indexer = getEntityNumberIndexer();
    REQUIRE(indexer.isIndexBuilt() == false);
    indexer.getEntityIdMap();
    REQUIRE(indexer.isIndexBuilt() == true);
}

TEST_CASE_METHOD(IndexerTestsFixture, "returns indexes", "[indexing]")
{
    auto indexer = getEntityNumberIndexer();

    TripleIndex first({ 0, 0, 1 });
    TripleIndex second({ 1, 1, 2 });
    TripleIndex third({ 0, 2, 2 });
    REQUIRE_THAT(indexer.getIndexes(),
                 Predicate<decltype(indexer.getIndexes())>(
                     [&](const auto& vec) -> bool {
                         return vec->at(0) == first &&
                            vec->at(1) == second &&
                            vec->at(2) == third;
                     }));
    REQUIRE_THAT(indexer.getEntityIdMap(),
                 Predicate<decltype(indexer.getEntityIdMap())>(
                     [](const auto& maps) -> bool {
                         return maps->size() == 3 &&
                             maps->at("/m/entity1") == 0 &&
                             maps->at("/m/entity2") == 1 &&
                             maps->at("/m/entity3") == 2; },
                     "entities were inserted."));
    REQUIRE_THAT(indexer.getRelationIdMap(),
        Predicate<decltype(indexer.getRelationIdMap())>(
            [](const auto& maps) -> bool {
                return maps->size() == 3 &&
                    maps->at("/produced_by") == 0 &&
                    maps->at("/country") == 1 &&
                    maps->at("/produced_in") == 2; },
            "relations were inserted."));
}

} // namespace kgekit
