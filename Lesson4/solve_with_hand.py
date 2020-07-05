import queue

que = queue.Queue()
que.put(((0, 0), 0))

# -------- Loop 1 ---------
que.empty() # False
que.get()   # ((0, 0), 0)

que.put(((1, 0), 1)) # 右へ
que.put(((0, 1), 1)) # 下へ
# -------------------------

# -------- Loop 2 ---------
que.empty() # False
que.get()   # ((1, 0), 1)

que.put(((2, 0), 2)) # 右へ
# -------------------------

# -------- Loop 3 ---------
que.empty() # False
que.get()   # ((0, 1), 1)

que.put(((0, 2), 2))
# -------------------------