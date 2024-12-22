import numpy as np
import pandas as pd

df = pd.read_csv("dataset/data.csv", na_values=["*"])
idxs = ["x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8", "x9"]
df[idxs] = df[idxs].replace(np.nan, -1)
df = df.ffill()
df.columns = ["stu", "tm", *idxs]
df.to_csv("build/output_preprocessed.csv", index=False)
