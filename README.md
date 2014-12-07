blizzard_hash_for_android
=========================

降低暴雪哈希的内存消耗（理论上牺牲碰撞率），千万量级别以内的数据，碰撞率和内存消耗都优与bloom filter

node 结点存放两个属性：

1)nHashA, nHashA取byte的前7位（1～127）,用以存储HashString返回值（long类型）的低7位。该值用来处理第一次碰撞的哈希值

2)bExists，bExists取byte的最后1位。用以标记某哈希地址是否已被占用

测试效果：
  2000000条数据，table_lenght=pow(2,21)（2097152）,占用内存大小为pow(2,21)*8（16M）=======》碰撞次数为0
