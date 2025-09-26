entry :: Int -> Int
entry 1 = 1
entry n = (1+) <$> entry $ (n-) <$> entry $ entry $ n - 1
