Record SearchSequenceSet(int ZipCode)
	{
			Index.open("TestIndex");
			//Input.seekg(ZipCode);
			string temp;
			int curLine = 0;
			string IndexOffset = "";

			while(getline(Input,temp))
			{
				curLine++;
				string IndexZip = "";

				int i = 0;
						while(temp.[i] != "!")
						{
							IndexZip = IndexZip + temp.[i];
							i++;
						}


							i++;
								if(ZipCode == IndexZip)
									{
										while(i < sizeof(temp))
											{
												IndexOffset = IndexOffset + temp[i];
											}
									}
								else if (IndexZip > ZipCode)
									{
										curLine--;
									}

				Block FBlock = GetBlock(IndexOffset);
					for(int i = 0; i <BLOCK_SIZE; i++)
						{
							if(FBlock.Records[i].Zip == ZipCode)
							{
								Return FBlock.Record[i];
							}

						}

			}

	}
