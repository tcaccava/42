'use client'

import { ScreenshotFileData } from "@/types/project";
import axios, { AxiosProgressEvent } from "axios";

export async function uploadScreenshots(
  projectId: number,
  screenshots: ScreenshotFileData[],
  onProgress: (event: AxiosProgressEvent) => void,
): Promise<any | null> {
  try {
	const formData = new FormData();
	let i = 0;
	for (const sc of screenshots) {
	  formData.append('file' + i, sc.data, sc.data.name);
	  i++;
	}

	const response = await axios.put(`/api/uploadScreenshots?id=${projectId}`, formData, {
	  onUploadProgress: onProgress,
	});

	if (response.status != axios.HttpStatusCode.Ok) {
	  throw new Error(JSON.stringify(response.data));
	}

	return response;
  }
  catch (error) {
	console.error('Failed to upload screenshots:', error);
	return null;
  }
}

export async function uploadSource(
  projectId: number,
  archive: File,
  onProgress: (event: AxiosProgressEvent) => void,
): Promise<any | null> {
  try {
	const formData = new FormData();
	formData.append('archive', archive)

	const response = await axios.put(`/api/uploadSource?id=${projectId}`, formData, {
	  onUploadProgress: onProgress,
	});

	if (response.status != axios.HttpStatusCode.Ok) {
	  throw new Error(JSON.stringify(response.data));
	}

	return response;
  }
  catch (error) {
	console.error('Failed to upload source:', error);
	return null;
  }
}