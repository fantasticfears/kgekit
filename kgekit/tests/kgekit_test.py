import unittest
import kgekit

class KgekitTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.index = (1, 2, 3)
        cls.triple_index = kgekit.TripleIndex()
        cls.triple_index.head = 1
        cls.triple_index.relation = 2
        cls.triple_index.tail = 3
        cls.literals = ("/m/1", "/m/2", "/m/3")
        cls.triple = kgekit.Triple()
        cls.triple.head = "/m/1"
        cls.triple.relation = "/m/2"
        cls.triple.tail = "/m/3"

    def test_triple_index(self):
        self.assertEqual(kgekit.TripleIndex(*self.index), self.triple_index)

    def test_triple(self):
        self.assertEqual(kgekit.Triple(*self.literals), self.triple)

    def test_get_triple_index(self):
        self.assertEqual(kgekit.get_triple_index("1 2 3", "hrt", ' '), kgekit.TripleIndex(1, 2, 3))
        self.assertEqual(kgekit.get_triple_index("1 2 3", "htr", ' '), kgekit.TripleIndex(1, 3, 2))

    def test_get_triple(self):
        self.assertEqual(kgekit.get_triple("/m/1 /m/2 /m/3", "hrt", ' '), kgekit.Triple("/m/1", "/m/2", "/m/3"))
        self.assertEqual(kgekit.get_triple("/m/1 /m/2 /m/3", "htr", ' '), kgekit.Triple("/m/1", "/m/3", "/m/2"))

if __name__ == '__main__':
    unittest.main()